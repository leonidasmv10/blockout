#include "Cube3D.h"

#include <iostream>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Application.h"
#include "GeometricTools.h"
#include "VertexArray.h"

Cube3D::Cube3D()
{
    this->position = glm::vec3(0.0f, 0.0f, 0.0f);
    this->scale = glm::vec3(1.0f, 1.0f, 1.0f);
    this->blending = 2;
    this->RandomColor();
    this->Init();
}

Cube3D::Cube3D(const glm::vec3& position, const glm::vec3& scale)
{
    this->position = position;
    this->scale = scale;
    this->blending = 2;
    this->RandomColor();
    this->Init();
}

Cube3D::~Cube3D()
{
}

void Cube3D::Init()
{
    vertexArray = std::make_shared<VertexArray>();
    vertexBuffer = std::make_shared<VertexBuffer>(GeometricTools::UnitCube3D, sizeof(GeometricTools::UnitCube3D));

    vertexArray->Bind();
    vertexBuffer->Bind();

    vertexArray->AttribPointer(3, 8, (void*)0);
    vertexArray->AttribPointer(2, 8, (void*)(3 * sizeof(float)));
    vertexArray->AttribPointer(3, 8, (void*)(5 * sizeof(float)));

    vertexBuffer->Unbind();
    vertexArray->Unbind();

    vertexArray->AddVertexBuffer(vertexBuffer);
}

void Cube3D::Render(const Shader& shader, const glm::mat4 projection, const glm::mat4 view)
{
    shader.Bind();
    glActiveTexture(GL_TEXTURE0);
    shader.UploadUniformInt("texture1", 0.0f);

    glm::mat4 model = glm::mat4(1.0f);

    shader.UploadUniformMat4("projection", projection);
    shader.UploadUniformMat4("view", view);

    shader.UploadUniformFloat("mixed", 0.6f);
    shader.UploadUniformFloat("blending", 0.4f);

    model = glm::scale(model, scale);
    model = glm::translate(model, position);

    shader.UploadUniformMat4("model", model);
    shader.UploadUniformVec4("color", color);

    vertexArray->Bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Cube3D::Debug() const
{
    std::cout << "Position\n";
    std::cout << "[" << position.x << "][" << position.y << "][" << position.z << "]\n";
}

void Cube3D::RandomColor()
{
    int r, g, b;
    do
    {
        r = rand() % 2;
        g = rand() % 2;
        b = rand() % 2;
    }
    while (r == g && r == b && g == b);

    color = {r, g, b, 1};
}
