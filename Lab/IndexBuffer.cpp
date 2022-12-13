#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const void* indices , GLsizei count)
{
    glGenBuffers(1, &IndexBufferID);
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count, indices,
                 GL_STATIC_DRAW);
    Unbind();
    this->Count = count;
}

IndexBuffer::~IndexBuffer()
{
}

void IndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferID);
}

void IndexBuffer::Unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
