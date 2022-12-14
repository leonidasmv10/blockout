#pragma once
#include <vector>
#include <glm/vec3.hpp>

#include "Cube3D.h"

class Blockout
{
public:
    void Init();
    void Update(const float &time,float &startFrame);
    void Render(const Shader& shader, const glm::mat4 projection, const glm::mat4 view) const;
    void Print() const;
    void MarkBlock(glm::vec3 pos);
    void AddBlock();
    void MoveBlock(int axis_x, int axis_y, int axis_z);
    void MoveBlock();
    bool ValidateLimits(glm::vec3 newPosition) const;
    bool HasBlock(glm::vec3 position) const;

private:
    int blocks[10][5][5];
    bool throwBlock = false;
    int depth = 0;
    float maxTime = 5.0f;
    
    std::vector<Cube3D*> cubes;
    Cube3D* currentCube = nullptr;
};
