#include "Cube3D.h"

#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Application.h"
#include "GeometricTools.h"
#include "VertexArray.h"

Cube3D::Cube3D()
{
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    scale = glm::vec3(1.0f, 1.0f, 1.0f);
    color = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
    this->Init();
}

Cube3D::Cube3D(const glm::vec3& position, const glm::vec3& scale)
{
    this->position = position;
    this->scale = scale;
    this->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    this->Init();
}

Cube3D::~Cube3D()
{
}

void Cube3D::Init()
{
    vertexArray = std::make_shared<VertexArray>();
    vertexBuffer = std::make_shared<VertexBuffer>(GeometricTools::UnitCube, sizeof(GeometricTools::UnitCube));

    vertexArray->Bind();
    vertexBuffer->Bind();

    vertexArray->AttribPointer(3, 5, (void*)0);
    vertexArray->AttribPointer(2, 5, (void*)(3 * sizeof(float)));

    vertexBuffer->Unbind();
    vertexArray->Unbind();

    vertexArray->AddVertexBuffer(vertexBuffer);
}

void Cube3D::Render(Shader& shader, PerspectiveCamera& camera)
{
    shader.Bind();
    glActiveTexture(GL_TEXTURE0);
    shader.UploadUniformInt("texture1", 0.0f);

    glm::mat4 model = glm::mat4(1.0f);
    const glm::mat4 projection = camera.GetProjectionMatrix();
    const glm::mat4 view = camera.GetViewMatrix();

    shader.UploadUniformMat4("projection", projection);
    shader.UploadUniformMat4("view", view);

    shader.UploadUniformFloat("mixed", 0.5f);
    shader.UploadUniformFloat("blending", 0.9f);

    model = glm::scale(model, scale);
    model = glm::translate(model, position);

    shader.UploadUniformMat4("model", model);
    shader.UploadUniformVec4("color", color);

    vertexArray->Bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
}