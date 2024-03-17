#include "Window.h"
#include "Camera.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <GLFW/glfw3.h>


Camera::Camera(vec3 position, float fov) : position(position), fov(fov), rotation(1.0f)
{
    updateVectors();
}

Camera::~Camera() = default;


void Camera::updateVectors()
{
    front = vec3(rotation * vec4(0, 0, -1, 1));
    right = vec3(rotation * vec4(1, 0, 0, 1));
    up = vec3(rotation * vec4(0, 1, 0, 1));
}

void Camera::rotate(float x, float y, float z)
{
    rotation = glm::rotate(rotation, z, vec3(0, 0, 1));
    rotation = glm::rotate(rotation, y, vec3(0, 1, 0));
    rotation = glm::rotate(rotation, x, vec3(1, 0, 0));

    updateVectors();
}

mat4 Camera::getProjection() const
{
    const auto* winData = static_cast<WindowData*>(glfwGetWindowUserPointer(Window::window));
    const float aspect = static_cast<float>(winData->width) / static_cast<float>(winData->height);
    return perspective(fov, aspect, 0.1f, 100.0f);
}

mat4 Camera::getView() const
{
    return lookAt(position, position + front, up);
}
