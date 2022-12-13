#pragma once
#include <vector>
#include <glm/vec3.hpp>

#include "Cube3D.h"

class Blockout
{
public:
    int block_[10][5][5];

    void Init();
    void Update(float deltatime);
    void Render(Shader& shader, PerspectiveCamera& camera);
    void Print();
    void MarkBlock(glm::vec3 pos);

    void AddBlock();
    void MoveBlock(int axis_x, int axis_y);

    std::vector<Cube3D*> cubes;
    Cube3D* currentCube;
};
