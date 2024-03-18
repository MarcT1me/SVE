/*
* Minecraft_cpp
*     Chunk.h
* Create on: 13 March 2024
*    Author: Marc_Time
* according to the lessons: Minecraft on C++ - MihailRis
*/
// ReSharper disable CppClangTidyModernizeMacroToEnum
// ReSharper disable CppClangTidyClangDiagnosticMismatchedTags
#pragma once
#ifndef VOXELS_CHUNK_H_
#define VOXELS_CHUNK_H_

#define CHUNK_W 32
#define CHUNK_H 128
#define CHUNK_D 32
#define CHUNK_VOL (CHUNK_W * CHUNK_H * CHUNK_D)

class voxel;

class Chunk
{
public:
    int x, y, z;
    voxel* voxels;

    mutable bool modified = true;
    
    Chunk(int xPos, int yPos, int zPos);
    ~Chunk();
};

#endif
