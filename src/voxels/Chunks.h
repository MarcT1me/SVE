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
};

#endif
