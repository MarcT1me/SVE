/*
* Voxel_cpp
*     png_loading.h
* Create on: 11 March 2024
*    Author: Marc_Time
* according to the lessons: Minecraft on C++ - MihailRis
*/
#pragma once
// ReSharper disable CppClangTidyClangDiagnosticMismatchedTags
#ifndef LOADERS_PNG_LOADING_H_
#define LOADERS_PNG_LOADING_H_

#include <string>

class Texture;

extern Texture* load_texture(const std::string& filename);

#endif
