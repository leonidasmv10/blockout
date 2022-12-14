#pragma once
#include "Shader.h"

class PhongLighting
{
public:
    static void SetLighting(Shader& shader, glm::vec3 position);
    inline static glm::vec3 lightPos = {-2.0f, 5.0f, 0.0f};
};
