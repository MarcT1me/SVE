﻿#include "Mesh.h"
#include <glad/glad.h>


Mesh::Mesh(const float* buffer, size_t vertices, const int* attrs) : vertices(vertices)
{
    int vertex_size = 0;
    for (int i = 0; attrs[i]; i++)
    {
        vertex_size += attrs[i];
    }
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex_size * vertices, buffer, GL_STATIC_DRAW);

    int offset = 0;
    for (int i = 0; attrs[i]; i++)
    {
        int size = attrs[i];
        glVertexAttribPointer(i, size, GL_FLOAT, GL_FALSE, vertex_size * sizeof(float),
                              reinterpret_cast<GLvoid*>(offset * sizeof(float))); // NOLINT(performance-no-int-to-ptr)
        glEnableVertexAttribArray(i);
        offset += size;
    }

    glBindVertexArray(0);
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}


void Mesh::draw(unsigned primitive) const
{
    glBindVertexArray(vao);
    glDrawArrays(primitive, 0, vertices);
    glBindVertexArray(0);
}
