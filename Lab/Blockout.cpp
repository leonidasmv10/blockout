#include "Blockout.h"

#include <iostream>
#include <glm/vec3.hpp>


void Blockout::Init()
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            for (int k = 0; k < 5; k++)
            {
                block_[i][j][k] = 0;
            }
        }
    }

    AddBlock();
    currentCube = cubes.back();
}

void Blockout::Update(float deltatime)
{
    auto pos = currentCube->GetPosition();
    if (pos.y < 1)
    {
        MarkBlock(pos);
        AddBlock();
        Print();
        currentCube = cubes.back();
    }
}

void Blockout::Render(Shader& shader, PerspectiveCamera& camera)
{
    for (const auto& cube : cubes)
    {
        cube->Render(shader, camera);
    }
}

void Blockout::Print()
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

void Blockout::MarkBlock(glm::vec3 pos)
{
    block_[static_cast<int>(pos.y)][static_cast<int>(pos.z + 2)][static_cast<int>(pos.x + 2)] = 1;
}

void Blockout::AddBlock()
{
    cubes.push_back(new Cube3D({0.0f, 10.5f, 0.0f}, {1.0f, 1.0f, 1.0f}));
    cubes.back()->SetColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
}

void Blockout::MoveBlock(int axis_x, int axis_y)
{
    auto pos = currentCube->GetPosition();
    currentCube->SetPosition({pos.x, pos.y - 1, pos.z});
}
