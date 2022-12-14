#pragma once
#include <glm/vec4.hpp>

#include "Quad2D.h"

class Scene
{
public:
    void Generate();
    void Render(const Shader& shader, const glm::mat4 projection, const glm::mat4 view);
private:
    glm::vec4 color1 = {1, 1, 1, 1};
    glm::vec4 color2 = {0, 1, 0, 1};

    const int N = 5;
    const int DEPTH = 10;
    std::vector<Quad2D*> quads;
    bool flag = false;
};
