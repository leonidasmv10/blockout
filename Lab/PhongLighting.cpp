#include "PhongLighting.h"

void PhongLighting::SetLighting(Shader& shader, glm::vec3 position)
{
    shader.Bind();

    shader.UploadUniformVec3("objectColor", 1.0f, 0.5f, 0.31f);
    shader.UploadUniformVec3("lightColor", 1.0f, 1.0f, 1.0f);
    shader.UploadUniformVec3("lightPos", lightPos);
    shader.UploadUniformVec3("viewPos", position);

    shader.Unbind();
}
