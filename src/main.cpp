/*
 * Voxel_engine
 *    main.spp
 * Create on: 9 March 2024
 *    Author: Marc_Time
 * according to the lessons: Minecraft on C++ - MihailRis
 */
#define GLEW_STATIC  // NOLINT(clang-diagnostic-unused-macros)

#include "graphics/Shader.h"
#include "graphics/Texture.h"
#include "window/Events.h"
#include "window/Window.h"
#include "loaders/png_loading.h"
#include "window/Camera.h"
#include "graphics/Mesh.h"
#include "voxels/Chunk.h"
#include "graphics/VoxelRenderer.h"
#include "voxels/Chunks.h"
// #include "voxels/voxel.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

using namespace glm;


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
    const Texture* texture = load_texture("res/block.png");
    if (texture == nullptr)
    {
        std::cerr << "failed to load texture" << std::endl;
        delete shader;
        Window::terminate();
        return 3;
    }

    /* Meshes */
    const auto* chunks = new Chunks(8, 1, 8);
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

    /* shaders uniforms */
    const GLint u_resolution_location = glGetUniformLocation(shader->id, "u_resolution");
    winData.u_resolution_location = u_resolution_location;
    shader->use();
    glUniform2f(u_resolution_location, static_cast<float>(winData.width), static_cast<float>(winData.height));

    /* main-loop */
    while (!Window::isShouldClose())
    {
        /* Events */
        if (Events::jPressed(GLFW_KEY_ESCAPE))
            Window::setShouldClose(true);
        if (Events::jPressed(GLFW_KEY_TAB))
            Events::toggleCursor();
        if (Events::pressed(GLFW_KEY_LEFT_CONTROL))
        {
            speed = 10.0f;
        }
        else
        {
            speed = 5.0f;
        }
        if (Events::pressed(GLFW_KEY_W))
        {
            camera->position += camera->front * deltaTime * speed;
        }
        if (Events::pressed(GLFW_KEY_S))
        {
            camera->position -= camera->front * deltaTime * speed;
        }
        if (Events::pressed(GLFW_KEY_D))
        {
            camera->position += camera->right * deltaTime * speed;
        }
        if (Events::pressed(GLFW_KEY_A))
        {
            camera->position -= camera->right * deltaTime * speed;
        }
        if (Events::pressed(GLFW_KEY_R))
        {
            camera->position += camera->up * deltaTime * speed;
        }
        if (Events::pressed(GLFW_KEY_F))
        {
            camera->position -= camera->up * deltaTime * speed;
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
        // build
        {
            vec3 end;
            vec3 norm;
            vec3 iend;
            voxel* vox = chunks->rayCast(camera->position, camera->front, 10.0, end, norm, iend);
            if (vox != nullptr)
            {
                if (Events::jClicked(GLFW_MOUSE_BUTTON_1))
                {
                    chunks->set(
                        static_cast<int>(iend.x),
                        static_cast<int>(iend.y),
                        static_cast<int>(iend.z),
                        0);
                }
                if (Events::jClicked(GLFW_MOUSE_BUTTON_2))
                {
                    chunks->set(
                        static_cast<int>(iend.x)+static_cast<int>(norm.x),
                        static_cast<int>(iend.y)+static_cast<int>(norm.y),
                        static_cast<int>(iend.z)+static_cast<int>(norm.z),
                        2);
                }
            }
        }

        /* chunks */
        Chunk* closes[27];
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
            Mesh* mesh = renderer.render(chunk, const_cast<const Chunk**>(closes));
            meshes[i] = mesh;
        }
        
        /* Update */
        const float current_time = glfwGetTime();
        deltaTime = current_time - lastTime;
        lastTime = current_time;
        // fps counter
        frames++;
        if (current_time - lastFpsTime >= 1.0)
        {
            std::cout << "\r" << std::flush;

            std::cout << "FPS: " << frames;
            std::cout << "\tdelta: " << deltaTime;
            std::cout << std::flush;

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

        Window::swapBuffers();
        Events::pullEvents();
    }
    delete shader;
    delete fill;
    delete crosshairMesh;
    delete texture;
    delete chunks;

    Window::terminate();
    return 0;
}
