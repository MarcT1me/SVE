#include "Shader.h"
// standard
#include <fstream>
#include <sstream>
#include <iostream>
// others
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>


Shader::Shader(unsigned id) : id(id)
{
}

Shader::~Shader()
{
    glDeleteProgram(id);
}


void Shader::use() const
{
    glUseProgram(id);
}

void Shader::uniformMatrix(std::string name, glm::mat4 matrix) const
{
    const GLuint transformLocation = glGetUniformLocation(id, name.c_str());
    glUniformMatrix4fv(transformLocation, 1, GL_FALSE, value_ptr(matrix));
}


Shader* load_shader(std::string vertexFile, std::string fragmentFile)
{
    /* Reading shader files */
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::badbit);
    try
    {
        vShaderFile.open(vertexFile);
        fShaderFile.open(fragmentFile);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        return nullptr;
    }
    const GLchar* vShaderCode = vertexCode.c_str();
    const GLchar* fShaderCode = fragmentCode.c_str();

    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX: compilation failed" << std::endl;
        std::cerr << infoLog << std::endl;
        return nullptr;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT: compilation failed" << std::endl;
        std::cerr << infoLog << std::endl;
        return nullptr;
    }

    GLuint id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);

    glGetShaderiv(id, GL_LINK_STATUS, &success);
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if (!success)
    {
        glGetShaderInfoLog(id, 512, nullptr, infoLog);
        std::cerr << "SHADER::PROHRAM: linking failed" << std::endl;
        std::cerr << infoLog << std::endl;

        glDeleteProgram(id);
        return nullptr;
    }

    return new Shader(id);
}
