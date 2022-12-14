#pragma once
#include <functional>
#include <string>

#include "Blockout.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Cube3D.h"
#include "Figure.h"
#include "Grid.h"
#include "PerspectiveCamera.h"
#include "Quad2D.h"
#include "Shader.h"


class Application
{
public:
    Application(const std::string& name, const std::string& version);
    ~Application();

    unsigned ParseArguments(int argc, char** argv);
    unsigned Init();
    unsigned Run();

    static const int width = 800;
    static const int height = 800;

private:
    void InputCallback();
    void Input(GLFWwindow* window);

    GLFWwindow* window;
    PerspectiveCamera camera;

    std::shared_ptr<Shader> shader;

    Blockout* game;
    

    std::vector<Quad2D*> quads;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    bool isBinding = false;

    std::function<void(int key, int scancode, int action, int mods)> key_input;
};
