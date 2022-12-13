#include "VertexBuffer.h"

#include <iostream>

#include "GeometricTools.h"

VertexBuffer::VertexBuffer(const void* vertices, GLsizei size)
{
    glGenBuffers(1, &VertexBufferID);

    Bind();
    glBufferData(GL_ARRAY_BUFFER, size, vertices,
                 GL_STATIC_DRAW);
    Unbind();
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &VertexBufferID);
}

void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
}

void VertexBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::BufferSubData(GLsizeiptr size, GLintptr offset, const void* data)
{
    glVertexAttribPointer(IdVertexAttrib, size, GL_FLOAT, GL_FALSE, offset * sizeof(float), data);
    glEnableVertexAttribArray(IdVertexAttrib++);
}
