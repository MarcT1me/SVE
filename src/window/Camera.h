/*
* Minecraft_cpp
*     Camera.h
* Create on: 13 March 2024
*    Author: Marc_Time
* according to the lessons: Minecraft on C++ - MihailRis
*/
// ReSharper disable CppClangTidyClangDiagnosticMismatchedTags
#pragma once
#ifndef WINDOW_CAMERA_H_
#define WINDOW_CAMERA_H_
// others
#include <glm/glm.hpp>


using namespace glm; // NOLINT(clang-diagnostic-header-hygiene)


class Camera
{
    void updateVectors();

public:
    // inspection vectors
    vec3 front{}, up{}, right{};
    // orientation in space
    vec3 position;
    float fov;
    mat4 rotation;
    // main
    Camera(vec3 position, float fov);
    ~Camera();
    // methods
    void rotate(float x, float y, float z);
    mat4 getProjection() const;
    mat4 getView() const;
};

#endif
