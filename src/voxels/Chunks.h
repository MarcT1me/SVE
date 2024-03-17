/*
* Minecraft_cpp
*     Chunks.h
* Create on: 15 March 2024
*    Author: Marc_Time
* according to the lessons: Minecraft on C++ - MihailRis
*/
#pragma once
// ReSharper disable CppClangTidyClangDiagnosticMismatchedTags
#ifndef VOXELS_CHUNKS_H_
#define VOXELS_CHUNKS_H_

# include <glm/glm.hpp>


class Chunk;
class voxel;


class Chunks
{
public:
    Chunk** chunks;
    size_t volume;
    unsigned int w, h, d;

    Chunks(int w, int h, int d);
    ~Chunks();

    Chunk* getChunk(int x, int y, int z) const;
    voxel* get(int x, int y, int z) const;
    void set(int x, int y, int z, int id) const;
	voxel* rayCast(glm::vec3 start, glm::vec3 dir, float maxLength, glm::vec3& end, glm::vec3& norm, glm::vec3& iend) const;
};

#endif
