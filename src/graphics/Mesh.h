/*
* Minecraft_cpp
*     Mesh.h
* Create on: 13 March 2024
*    Author: Marc_Time
* according to the lessons: Minecraft on C++ - MihailRis
*/
#pragma once
// ReSharper disable CppClangTidyClangDiagnosticMismatchedTags
#ifndef GRAPHICS_MESH_H_
#define GRAPHICS_MESH_H_

class Mesh
{
    unsigned int vao;
    unsigned int vbo;
    size_t vertices;
public:
    Mesh(const float* buffer, size_t vertices, const int* attrs);
    ~Mesh();

    void draw(unsigned int primitive);
};

#endif
