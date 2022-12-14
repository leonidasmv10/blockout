#pragma once
#include <glm/vec3.hpp>

#include "PerspectiveCamera.h"
#include "Shader.h"
#include "VertexArray.h"

class Quad2D
{
public:
    Quad2D();
    Quad2D(const glm::vec3& position,const glm::vec3& rotation, const glm::vec3& scale);
    ~Quad2D();
    void Init();
    void Render(const Shader& shader, const glm::mat4 projection, const glm::mat4 view);

    void SetPosition(const glm::vec3& position)
    {
        this->position = position;
    }

    void SetScale(const glm::vec3& scale)
    {
        this->scale = scale;
    }

    void SetRotation(const glm::vec3& rotation)
    {
        this->rotation = rotation;
    }

    void SetColor(const glm::vec4& color)
    {
        this->color = color;
    }

    glm::vec3 GetPosition() const
    {
        return this->position;
    }

    glm::vec3 GetScale() const
    {
        return this->scale;
    }

    glm::vec4 GetColor() const
    {
        return this->color;
    }

private:
    unsigned int EBO;

    std::shared_ptr<VertexArray> vertexArray;
    std::shared_ptr<VertexBuffer> vertexBuffer;
    std::shared_ptr<IndexBuffer> elementBuffer;

    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;
    glm::vec4 color;
};
