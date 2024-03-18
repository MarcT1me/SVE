/*
 * Voxel_engine
 *    main.spp
 * Create on: 9 March 2024
 *    Author: Marc_Time
 * according to the lessons: Minecraft on C++ - MihailRis
 */
#define GLEW_STATIC  // NOLINT(clang-diagnostic-unused-macros)
// standard
#include <iostream>
// other
#include <glm/glm.hpp>
#include <glm/ext.hpp>

using namespace glm;
// engine
#include "window/Window.h"
#include "window/Events.h"
// shaders
#include "graphics/Shader.h"
#include "graphics/Texture.h"
#include "loaders/png_loading.h"
// space
#include "window/Camera.h"
#include "graphics/Mesh.h"
// voxels
#include "voxels/Chunk.h"
#include "voxels/Chunks.h"
#include "graphics/VoxelRenderer.h"
#include "voxels/voxel.h"


constexpr float crosshairVertices[] = {
    // x       y
    -0.01f, -0.01f,
     0.01f,  0.01f,

    -0.01f,  0.01f,
     0.01f, -0.01f,
};

constexpr int crosshairAttrs[] = {
    2, 0,
};

/*
constexpr int cubeAttrs[] = {
    3, 2, 0,
};
*/


int main()
{
    /* Create window config */
    WindowData winData = WindowData{};
    winData.width = 1600;
    winData.height = 900;

    /* Init engine */
    Window::initialise(winData, "Voxel Game");
    Events::initialize();

    /* Setting OpenGl data */
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    /* Shaders */
    const Shader* shader = load_shader("res/main.vert", "res/main.frag");
    if (shader == nullptr)
    {
        std::cerr << "Failed load shader" << std::endl;
        Window::terminate();
        return 1;
    } // load scene shader
    const Shader* fill = load_shader("res/fill.vert", "res/fill.frag");
    if (fill == nullptr)
    {
        std::cerr << "Failed to load crosshair shader" << std::endl;
        Window::terminate();
        return 2;
    } // load crosshair shader

    /* Texture */
    const Texture* texture = load_texture("res/tex_atl2.png");
    if (texture == nullptr)
    {
        std::cerr << "failed to load texture" << std::endl;
        delete shader;
        Window::terminate();
        return 3;
    }

    /* Meshes */
    const auto* chunks = new Chunks(4, 1, 4);
    Mesh** meshes = new Mesh*[chunks->volume];
    for (size_t i = 0; i < chunks->volume; i++)
    {
        meshes[i] = nullptr;
    }
    const VoxelRenderer renderer(1024 * 1024 * 8); // NOLINT(bugprone-implicit-widening-of-multiplication-result)
    // crosshair
    const Mesh* crosshairMesh = new Mesh(crosshairVertices, 4, crosshairAttrs);

    /* camera */
    auto* camera = new Camera(vec3(4 * 16 * 0, 10, 4 * 16 * 0), radians(60.0f));

    /* variables */
    float lastFpsTime = glfwGetTime();
    int frames = 0;
    float lastTime = lastFpsTime;
    float deltaTime = 0.0f;
    float camX = 0.0f;
    float camY = 0.0f;
    float speed;
    uint8_t block_id = 4;

    /* shaders uniforms */
    const GLint u_resolution_location = glGetUniformLocation(shader->id, "u_resolution");
    winData.u_resolution_location = u_resolution_location;
    shader->use();
    glUniform2f(u_resolution_location, static_cast<float>(winData.width), static_cast<float>(winData.height));

    /* main-loop */
    while (!Window::isShouldClose())
    {
        std::cout << "\r" << std::flush;
        /* Events */
        if (Events::jPressed(GLFW_KEY_ESCAPE))
            Window::setShouldClose(true);
        if (Events::jPressed(GLFW_KEY_TAB))
            Events::toggleCursor();
        // move
        {
            // speed
            if (Events::pressed(GLFW_KEY_LEFT_CONTROL))
            {
                speed = 10.0f;
            }
            else
            {
                speed = 5.0f;
            }
            // steps
            if (Events::pressed(GLFW_KEY_W))
            {
                camera->position += normalize(vec3(camera->front.x, 0, camera->front.z)) * deltaTime * speed;
            }
            if (Events::pressed(GLFW_KEY_S))
            {
                camera->position -= normalize(vec3(camera->front.x, 0, camera->front.z)) * deltaTime * speed;
            }
            if (Events::pressed(GLFW_KEY_D))
            {
                camera->position += normalize(vec3(camera->right.x, 0, camera->right.z)) * deltaTime * speed;
            }
            if (Events::pressed(GLFW_KEY_A))
            {
                camera->position -= normalize(vec3(camera->right.x, 0, camera->right.z)) * deltaTime * speed;
            }
            if (Events::pressed(GLFW_KEY_SPACE))
            {
                camera->position += normalize(vec3(0, camera->up.y, 0)) * deltaTime * speed;
            }
            if (Events::pressed(GLFW_KEY_LEFT_SHIFT))
            {
                camera->position -= normalize(vec3(0, camera->up.y, 0)) * deltaTime * speed;
            }
            if (Events::_cursor_locked)
            {
                camY += -Events::deltaY / winData.height * 2;
                camX += -Events::deltaX / winData.height * 2;

                if (camY < -radians(89.0f))
                {
                    camY = -radians(89.0f);
                }
                if (camY > radians(89.0f))
                {
                    camY = radians(89.0f);
                }

                camera->rotation = mat4(1.0f);
                camera->rotate(camY, camX, 0);
            }
        }
        // build
        {
            // RayCast voxel
            vec3 end;
            vec3 norm;
            vec3 iend;
            voxel* vox = chunks->rayCast(camera->position, camera->front, 10.0, end, norm, iend);
            // manipulate with voxels
            if (vox != nullptr)
            {
                std::cout << "block found; id: " << static_cast<int>(vox->id) << "    selected block id: " << static_cast<int>(block_id);
                if (Events::jClicked(SVE_MOUSE_LEFT_BUTTON))
                {
                    chunks->set(
                        static_cast<int>(iend.x),
                        static_cast<int>(iend.y),
                        static_cast<int>(iend.z),
                        0);
                }
                if (Events::jClicked(SVE_MOUSE_RIGHT_BUTTON))
                {
                    chunks->set(
                        static_cast<int>(iend.x)+static_cast<int>(norm.x),
                        static_cast<int>(iend.y)+static_cast<int>(norm.y),
                        static_cast<int>(iend.z)+static_cast<int>(norm.z),
                        block_id);
                }
                // change selected block
                if (Events::jScroll(SVE_MOUSE_SCROLL_UP))
                    block_id++;
                if (Events::jScroll(SVE_MOUSE_SCROLL_DOWN))
                    block_id--;
            }
        }
        
        /* Update */
        Chunk* closes[27]; // scene
        for (size_t i = 0; i < chunks->volume; i++)
        {
            Chunk* chunk = chunks->chunks[i];
            if (!chunk->modified)
                continue;
            chunk->modified = false;
            if (meshes[i] != nullptr)
            {
                delete meshes[i];
            }
            
            for (auto& close : closes)
                close = nullptr;
            for (size_t j = 0; j < chunks->volume; j++)
            {
                Chunk* other = chunks->chunks[j];

                int ox = other->x - chunk->x;
                int oy = other->y - chunk->y;
                int oz = other->z - chunk->z;

                if (abs(ox) > 1 || abs(oy) > 1 || abs(oz) > 1)
                    continue;

                ox += 1;
                oy += 1;
                oz += 1;
                closes[(oy * 3 + oz) * 3 + ox] = other;
            }
            Mesh* mesh = renderer.render(chunk, const_cast<const Chunk**>(closes), true);
            meshes[i] = mesh;
        }
        // delta and fps timer
        const float current_time = glfwGetTime();
        deltaTime = current_time - lastTime;
        lastTime = current_time;
        // fps counter
        frames++;
        if (current_time - lastFpsTime >= 1.0)
        {
            std::cout << "    FPS: " << frames;
            std::cout << "    delta: " << deltaTime << "\t\t\t\t\t\t\t";

            frames = 0;
            lastFpsTime = current_time;
        }

        /*  Render  */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // use shaders, texture and VAO
        shader->use();
        shader->uniformMatrix("projView", camera->getProjection() * camera->getView());
        texture->bind();
        for (size_t i = 0; i < chunks->volume; i++)
        {
            const auto* chunk = chunks->chunks[i];
            auto* mesh = meshes[i];
            const mat4 model_matrix = translate(
                mat4(1.0f),
                vec3(chunk->x * CHUNK_W+0.5f, chunk->y * CHUNK_H+0.5f, chunk->z * CHUNK_D+0.5f)
                );
            shader->uniformMatrix("model_matrix", model_matrix);
            mesh->draw(GL_TRIANGLES);
        }
        // crosshair
        fill->use();
        crosshairMesh->draw(GL_LINES);

        /* main end */
        Window::swapBuffers();
        Events::pullEvents();
        std::cout << std::flush;
    }
    delete shader;
    delete fill;
    delete crosshairMesh;
    delete texture;
    delete chunks;

    Window::terminate();
    return 0;
}
