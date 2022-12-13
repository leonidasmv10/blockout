#include <glad/glad.h>

class IndexBuffer
{
public:
    // Constructor. It initializes with a data buffer and the size of it.
    // Note that the buffer will be bound on construction and that the.size is
    // given in number of elements, not bytes
    IndexBuffer(const void* indices , GLsizei count);
    ~IndexBuffer();

    // Bind the vertex buffer
    void Bind() const;

    // Unbind the vertex buffer
    void Unbind() const;
    
    // Get the number of elements
    inline GLuint GetCount() const { return Count; }

private:
    GLuint IndexBufferID;
    GLuint Count;
};
