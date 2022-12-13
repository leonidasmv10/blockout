#include "Application.h"

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GeometricTools.h"
#include "Grid.h"
#include "TextureManager.h"

Application::Application(const std::string& name, const std::string& version)
{
    glfwSetErrorCallback([](int code, const char* description)
    {
        glfwGetError(&description);
        std::cerr << "Error " << "0x" << std::hex << code << ':' << description << "\n";
    });

    // Initialization of glfw.
    if (!glfwInit())
    {
        std::cin.get();
        return;
    }

    // Create a window
    glfwWindowHint(GLFW_RESIZABLE, false);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
    if (window == nullptr)
    {
        glfwTerminate();
        std::cin.get();
        return;
    }

    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        auto w = static_cast<Application*>(glfwGetWindowUserPointer(window));
        if (w->key_input) w->key_input(key, scancode, action, mods);
    });
    glfwSetWindowUserPointer(window, this);

    //Set the OpenGL context
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return;
    }

    InputCallback();

    // Enable capture of debug output.
    glEnable(GL_DEBUG_OUTPUT);
    // glDebugMessageCallback(
    //     [](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
    //        const GLchar* message, const void* userParam)
    //     {
    //         std::cerr << "GL CALLBACK:" << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "") <<
    //             "type = 0x" << type <<
    //             ", severity = 0x" << severity <<
    //             ", message =" << message << "\n";
    //     }, 0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Application::~Application()
{
    glfwTerminate();
}

unsigned Application::ParseArguments(int argc, char** argv)
{
    return 1;
}

unsigned Application::Init()
{
    // Print OpenGL data
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << "\n";
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";

   
    shader2 = std::make_shared<Shader>(std::string(SHADERS_DIR) + "model.vert",
                                       std::string(SHADERS_DIR) + "model.frag");


    shader2->Bind();

    grid = new Grid();

    grid->Init();

    shader2->Unbind();


    camera = PerspectiveCamera();
    camera.SetFrustrum(PerspectiveCamera::Frustrum(45.0f, width, height, 0.1f, 100.0f));
    camera.SetMovement(0, 0.0);

    // camera.SetPosition(glm::vec3(0.0f,0.0f,0.0f));

    cube = new Cube3D(glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(1.0f));

    cube2 = new Cube3D(glm::vec3(2.0f, 10.5f, 0.0f), glm::vec3(1.0f));
    cube2->SetColor(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
    

    TextureManager::GetInstance()->LoadTexture2DRGBA("cube_texture",
                                                     std::string(TEXTURES_DIR) + "cube_texture.png",
                                                     0, true);

    TextureManager::GetInstance()->LoadTexture2DRGBA("floor_texture",
                                                     std::string(TEXTURES_DIR) + "floor_texture.png",
                                                     1, false);

    

    return 1;
}

unsigned Application::Run()
{
    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        const float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        Input(window);

        glClearColor(0.0f, 0.31f, 0.87f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // board->Render(*shader.get(), camera);

        glm::mat4 model = glm::mat4(1.0f);
        const glm::mat4 projection = camera.GetProjectionMatrix();
        const glm::mat4 view = camera.GetViewMatrix();
        model = glm::translate(model, glm::vec3(-2.5f, 0.0f, -2.5f));
        model = glm::scale(model, glm::vec3(5.0f));

        shader2->Bind();

        shader2->UploadUniformMat4("projection", projection);
        shader2->UploadUniformMat4("view", view);
        shader2->UploadUniformMat4("model", model);

        grid->Render();

        shader2->Unbind();

        cube->Render(*shader2, camera);
        cube2->Render(*shader2, camera);

        glfwSwapBuffers(window);
    }

    return 1;
}

void Application::InputCallback()
{
    key_input = [&](int key, int sancode, int action, int mods)
    {
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        {
            auto pos = cube2->GetPosition();
            cube2->SetPosition(glm::vec3(pos.x, pos.y - 1.0f, pos.z));
        }
    };
}

void Application::Input(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        camera.SetMovement(deltaTime, 1.f);

    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
        camera.SetMovement(deltaTime, -1.f);

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        camera.SetAngle(camera.GetAngle() - 1.0f * deltaTime);

    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        camera.SetAngle(camera.GetAngle() + 1.0f * deltaTime);
}
