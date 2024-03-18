#include "Texture.h"
// others
#include <glad/glad.h>


Texture::Texture(unsigned id, int width, int height) : id(id), width(width), height(height)
{
}

Texture::~Texture()
{
    glDeleteTextures(1, &id);
}


void Texture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, id);
}
