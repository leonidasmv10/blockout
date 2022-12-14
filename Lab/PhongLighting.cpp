#include "PhongLighting.h"


void PhongLighting::Update(const float deltaTime)
{
    movement.x += velocity * deltaTime;
    movement.z += velocity * deltaTime;
    
    lightPos = glm::vec3(sin(movement.x) * radius, 6.0f, cos(movement.z) * radius);
}

void PhongLighting::SetLighting(const Shader& shader, const glm::vec3 position)
{
    shader.Bind();
    
    shader.UploadUniformVec3("lightColor", lightColor);
    shader.UploadUniformVec3("lightPos", lightPos);
    shader.UploadUniformVec3("viewPos", position);

    shader.Unbind();
}
