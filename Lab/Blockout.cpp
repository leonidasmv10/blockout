#include "Blockout.h"

#include <iostream>
#include <glm/vec3.hpp>


void Blockout::Init()
{
    for (auto& i : block_)
    {
        for (auto& j : i)
        {
            for (int& k : j)
            {
                k = 0;
            }
        }
    }

    AddBlock();
    currentCube = cubes.back();
}

void Blockout::Update()
{
    if (!flag) return;

    const auto pos = currentCube->GetPosition();

    for (int i = 0; i < 10; i++)
    {
        if (!HasBlock({pos.x, i, pos.z}))
        {
            depth = i;
            break;
        }
    }


    const glm::vec3 target = {pos.x, depth + 0.5f, pos.z};
    glm::vec3 lerp = glm::mix(pos, target, 0.2f);
    currentCube->SetPosition({lerp.x, lerp.y, lerp.z});
    
    if (pos.y <= 0.51f + depth)
    {
        flag = false;
        MarkBlock(pos);
        AddBlock();
        Print();
        currentCube = cubes.back();
    }
}

void Blockout::Render(Shader& shader, PerspectiveCamera& camera) const
{
    for (const auto& cube : cubes)
    {
        cube->Render(shader, camera);
    }
}

void Blockout::Print() const
{
    for (int i = 0; i < 10; i++)
    {
        std::cout << "Level: " << i + 1 << "\n";
        for (int j = 0; j < 5; j++)
        {
            for (int k = 0; k < 5; k++)
            {
                std::cout << block_[i][j][k];
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }
}

void Blockout::MarkBlock(const glm::vec3 pos)
{
    block_[static_cast<int>(pos.y)][static_cast<int>(pos.z + 2)][static_cast<int>(pos.x + 2)] = 1;
}

void Blockout::AddBlock()
{
    cubes.push_back(new Cube3D({0.0f, 9.5f, 0.0f}, {1.0f, 1.0f, 1.0f}));
    cubes.back()->SetColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
}

void Blockout::MoveBlock(int axis_x, int axis_y, int axis_z)
{
    if (flag) return;
    const auto pos = currentCube->GetPosition();
    const glm::vec3 newPosition = {pos.x + axis_x, pos.y + axis_y, pos.z + axis_z};

    if (ValidateLimits(newPosition))
    {
        currentCube->SetPosition(newPosition);
        currentCube->Debug();
    }
}


void Blockout::MoveBlock()
{
    flag = true;
}


bool Blockout::ValidateLimits(const glm::vec3 newPosition) const
{
    return newPosition.x >= -2 && newPosition.x <= 2 &&
        newPosition.y >= 0 && newPosition.y <= 9.5f &&
        newPosition.z >= -2 && newPosition.z <= 2;
}

bool Blockout::HasBlock(const glm::vec3 position) const
{
    // std::cout << position.x << "-" << position.y << "-" << position.z << "\n";
    return block_[static_cast<int>(position.y)][static_cast<int>(position.z + 2)][static_cast<int>(position.x + 2)];
}
