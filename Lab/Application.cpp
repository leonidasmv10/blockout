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

    shader = std::make_shared<Shader>(std::string(SHADERS_DIR) + "model.vert",
                                      std::string(SHADERS_DIR) + "model.frag");

    srand(time(NULL));

    shader->Bind();

    glm::vec4 green = {1, 1, 1, 1};
    glm::vec4 black = {0, 0, 0, 1};

    bool flag = false;

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            auto q = new Quad2D({i * 1 - 2.0f, 0.0f, j * 1 - 2.0f}, {0.0f, 0.0f, 0.0f}, {0.5, 0.1f, 0.5f});
            q->Init();
            if (flag)
                q->SetColor(green);
            else
                q->SetColor(black);
            flag = !flag;
            quads.push_back(q);
        }
        // flag = !flag;
    }

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            auto q = new Quad2D({2.5f, j + 0.5f, 2.f - i}, {0.0f, 0.0f, 90.0f}, {0.5, 0.1f, 0.5f});
            q->Init();
            if (flag)
                q->SetColor(green);
            else
                q->SetColor(black);
            flag = !flag;
            quads.push_back(q);
        }
        flag = !flag;
    }
    flag = !flag;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            auto q = new Quad2D({-2.5f, j + 0.5f, 2.f - i}, {0.0f, 0.0f, 90.0f}, {0.5, 0.1f, 0.5f});
            q->Init();
            if (flag)
                q->SetColor(green);
            else
                q->SetColor(black);
            flag = !flag;
            quads.push_back(q);
        }
        flag = !flag;
    }

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            auto q = new Quad2D({2.f - i, 0.5f + j, 2.5f}, {90.0f, 0.0f, 00.0f}, {0.5, 0.1f, 0.5f});
            q->Init();
            if (flag)
                q->SetColor(green);
            else
                q->SetColor(black);
            flag = !flag;
            quads.push_back(q);
        }
        flag = !flag;
    }
    flag = !flag;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            auto q = new Quad2D({2.f - i, 0.5f + j, -2.5f}, {90.0f, 0.0f, 00.0f}, {0.5, 0.1f, 0.5f});
            q->Init();
            if (flag)
                q->SetColor(green);
            else
                q->SetColor(black);
            flag = !flag;
            quads.push_back(q);
        }
        flag = !flag;
    }


    game = new Blockout();

    game->Init();


    shader->Unbind();


    camera = PerspectiveCamera();
    camera.SetFrustrum(PerspectiveCamera::Frustrum(45.0f, width, height, 0.1f, 100.0f));
    // camera.SetMovement(0, 0.0);


    TextureManager::GetInstance()->LoadTexture2DRGBA("cube_texture",
                                                     std::string(TEXTURES_DIR) + "cube_texture.png",
                                                     0, true);

    TextureManager::GetInstance()->LoadTexture2DRGBA("floor_texture",
                                                     std::string(TEXTURES_DIR) + "marble.jpg",
                                                     1, false);


    shader->Bind();
    shader->UploadUniformBool("isBinding", false);
    shader->Unbind();

    return 1;
}

unsigned Application::Run()
{
    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        const float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        Input(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        game->Update();

        // board->Render(*shader.get(), camera);

        glm::mat4 model = glm::mat4(1.0f);
        const glm::mat4 projection = camera.GetProjectionMatrix();
        const glm::mat4 view = camera.GetViewMatrix();
        model = glm::translate(model, glm::vec3(-2.5f, 0.0f, -2.5f));
        model = glm::scale(model, glm::vec3(5.0f));

        shader->Bind();


        shader->UploadUniformVec3("objectColor", 1.0f, 0.5f, 0.31f);
        shader->UploadUniformVec3("lightColor", 1.0f, 1.0f, 1.0f);
        shader->UploadUniformVec3("lightPos", lightPos);
        shader->UploadUniformVec3("viewPos", camera.GetPosition());


        shader->UploadUniformVec4("color", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
        shader->UploadUniformMat4("projection", projection);
        shader->UploadUniformMat4("view", view);
        shader->UploadUniformMat4("model", model);


        shader->Unbind();

        game->Render(*shader, camera);

        for (const auto& quad : quads)
        {
            quad->Render(*shader, camera);
        }


        glfwSwapBuffers(window);
    }

    return 1;
}

void Application::InputCallback()
{
    key_input = [&](int key, int sancode, int action, int mods)
    {
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            game->MoveBlock();
        }

        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        {
            game->MoveBlock(0, -1, 0);
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            game->MoveBlock(-1, 0, 0);
        }

        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        {
            game->MoveBlock(1, 0, 0);
        }

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        {
            game->MoveBlock(0, 0, -1);
        }

        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            game->MoveBlock(0, 0, 1);
        }
        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        {
            isBinding = !isBinding;
            shader->UploadUniformBool("isBinding", isBinding);
        }
    };
}

void Application::Input(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
