/*
* Voxel_cpp
*     Shader.h
* Create on: 10 March 2024
*    Author: Marc_Time
* according to the lessons: Minecraft on C++ - MihailRis
*/
#pragma once
// ReSharper disable CppClangTidyClangDiagnosticMismatchedTags
#ifndef GRAPHICS_SHADER_H_
#define GRAPHICS_SHADER_H_

#include <string>
#include <glm/fwd.hpp>

class Shader
{
public:
    unsigned int id;
    Shader(unsigned int id);
    ~Shader();

    void use() const;
    void uniformMatrix(std::string name, glm::mat4 matrix) const;
};

extern Shader* load_shader(std::string vertexFile, std::string fragmentFile);

#endif
