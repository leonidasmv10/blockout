#pragma once
#include <glad/glad.h>
class Grid
{
public:
    void Init();
    void Render();

    int slices = 5;

    GLuint vao;

    GLuint lenght;
};
