#include "Quad2D.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Application.h"
#include "GeometricTools.h"

Quad2D::Quad2D()
{
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    scale = glm::vec3(1.0f, 1.0f, 1.0f);
    color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    this->Init();
}

Quad2D::Quad2D(const glm::vec3& position,const glm::vec3& rotation, const glm::vec3& scale)
{
    this->position = position;
    this->rotation = rotation;
    this->scale = scale;
    this->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    this->Init();
}

Quad2D::~Quad2D()
{
}

void Quad2D::Init()
{
    vertexArray = std::make_shared<VertexArray>();
    vertexBuffer = std::make_shared<VertexBuffer>(GeometricTools::UnitQuad2D, sizeof(GeometricTools::UnitQuad2D));
    elementBuffer = std::make_shared<IndexBuffer>(GeometricTools::InidicesQuad2D, sizeof(GeometricTools::UnitQuad2D));

    vertexArray->Bind();
    vertexBuffer->Bind();
    elementBuffer->Bind();

    vertexArray->AttribPointer(3, 5, (void*)0);
    vertexArray->AttribPointer(2, 5, (void*)(3 * sizeof(float)));

    vertexBuffer->Unbind();
    vertexArray->Unbind();
    elementBuffer->Unbind();

    vertexArray->AddVertexBuffer(vertexBuffer);
}

void Quad2D::Render(Shader& shader, PerspectiveCamera& camera)
{
    shader.Bind();

    glActiveTexture(GL_TEXTURE1);
    shader.UploadUniformInt("texture1", 1.0f);

    glm::mat4 model = glm::mat4(1.0f);
    const glm::mat4 projection = camera.GetProjectionMatrix();
    const glm::mat4 view = camera.GetViewMatrix();

    shader.UploadUniformMat4("projection", projection);
    shader.UploadUniformMat4("view", view);

    shader.UploadUniformFloat("mixed", 0.9f);
    shader.UploadUniformFloat("blending", 1.0f);

    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
    model = glm::scale(model, scale);

    shader.UploadUniformMat4("model", model);
    shader.UploadUniformVec4("color", color);

    vertexArray->Bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
