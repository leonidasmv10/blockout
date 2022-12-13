#include "VertexArray.h"

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_vertexArrayID);
}

VertexArray::~VertexArray()
{
    glDisableVertexAttribArray(0);
    glDeleteVertexArrays(1, &m_vertexArrayID);
}

void VertexArray::Bind() const
{
    glBindVertexArray(m_vertexArrayID);
}

void VertexArray::Unbind() const
{
    glBindVertexArray(0);
}

void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
{
    this->VertexBuffers.push_back(vertexBuffer);
}

void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
{
    this->IdxBuffer = indexBuffer;
}

void VertexArray::AttribPointer(GLsizeiptr size, GLintptr offset, const void* data)
{
    glVertexAttribPointer(IdVertexAttrib, size, GL_FLOAT, GL_FALSE, offset * sizeof(float), data);
    glEnableVertexAttribArray(IdVertexAttrib++);
}
