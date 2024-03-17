/*
* Minecraft_cpp
*     VoxelRenderer.h
* Create on: 13 march 2024
*    Author: Marc_Time
* according to the lessons: Minecraft on C++ - MihailRis
*/
#pragma once
// ReSharper disable CppClangTidyClangDiagnosticMismatchedTags
#ifndef GRAPHICS_VOXELRENDERER_H_
#define GRAPHICS_VOXELRENDERER_H_

class Mesh;
class Chunk;

class VoxelRenderer
{
    float* buffer;
    size_t capacity;  // NOLINT(clang-diagnostic-unused-private-field)
public:
    VoxelRenderer(size_t capacity);
    ~VoxelRenderer();

    Mesh* render(Chunk* chunk, const Chunk** chunks);
};


#endif
