/*
* Voxel_cpp
*     Texture.h
* Create on: 11 March 2024
*    Author: Marc_Time
* according to the lessons: Minecraft on C++ - MihailRis
*/
#pragma once
// ReSharper disable CppClangTidyClangDiagnosticMismatchedTags
#ifndef GRAPHICS_TEXTURE_H_
#define GRAPHICS_TEXTURE_H_

#include <string>

class Texture
{
public:
    unsigned int id;
    int width, height;
    Texture(unsigned id, int width, int height);
    ~Texture();

    void bind() const;
};

#endif
