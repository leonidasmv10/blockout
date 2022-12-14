#pragma once
#include "Shader.h"

class PhongLighting
{
public:
    static void Update(const float deltaTime);
    static void SetLighting(const Shader& shader, glm::vec3 position);
    inline static glm::vec3 lightPos = {0.0f, 0.0f, 0.0f};
    inline static glm::vec3 lightColor = {1.0f, 1.0f, 1.0f};
    inline static glm::vec3 movement = {0.0f, 0.0f, 0.0f};
    inline static float velocity = 1.0f;
    inline static float radius = 4.0f;
};
