#pragma once
#include <glad/glad.h>

class Figure
{
public:
    void Init();
    void Render();

    int slices = 1;

    GLuint vao;

    GLuint lenght;
};
