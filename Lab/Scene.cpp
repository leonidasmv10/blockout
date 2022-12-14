#include "Scene.h"

#include "Quad2D.h"

void Scene::Generate()
{
    //PLANE
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            auto q = new Quad2D({i * 1 - 2.0f, 0.0f, j * 1 - 2.0f}, {0.0f, 0.0f, 0.0f}, {0.5, 0.1f, 0.5f});
            q->Init();
            if (flag)
                q->SetColor(color1);
            else
                q->SetColor(color2);
            flag = !flag;
            quads.push_back(q);
        }
    }

    //RIGHT
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < DEPTH; j++)
        {
            auto q = new Quad2D({2.5f, j + 0.5f, 2.f - i}, {0.0f, 0.0f, 90.0f}, {0.5, 0.1f, 0.5f});
            q->Init();
            if (flag)
                q->SetColor(color1);
            else
                q->SetColor(color2);
            flag = !flag;
            quads.push_back(q);
        }
        flag = !flag;
    }
    flag = !flag;

    //LEFT
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < DEPTH; j++)
        {
            auto q = new Quad2D({-2.5f, j + 0.5f, 2.f - i}, {0.0f, 0.0f, 90.0f}, {0.5, 0.1f, 0.5f});
            q->Init();
            if (flag)
                q->SetColor(color1);
            else
                q->SetColor(color2);
            flag = !flag;
            quads.push_back(q);
        }
        flag = !flag;
    }

    //UP
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < DEPTH; j++)
        {
            auto q = new Quad2D({2.f - i, 0.5f + j, 2.5f}, {90.0f, 0.0f, 00.0f}, {0.5, 0.1f, 0.5f});
            q->Init();
            if (flag)
                q->SetColor(color1);
            else
                q->SetColor(color2);
            flag = !flag;
            quads.push_back(q);
        }
        flag = !flag;
    }
    flag = !flag;

    //DOWN
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < DEPTH; j++)
        {
            auto q = new Quad2D({2.f - i, 0.5f + j, -2.5f}, {90.0f, 0.0f, 00.0f}, {0.5, 0.1f, 0.5f});
            q->Init();
            if (flag)
                q->SetColor(color1);
            else
                q->SetColor(color2);
            flag = !flag;
            quads.push_back(q);
        }
        flag = !flag;
    }
}

void Scene::Render(const Shader& shader, const glm::mat4 projection, const glm::mat4 view)
{
    for (const auto& quad : quads)
    {
        quad->Render(shader, projection, view);
    }
}
