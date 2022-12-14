#include "Application.h"

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

    shaderCube = std::make_shared<Shader>(std::string(SHADERS_DIR) + "model.vert",
                                          std::string(SHADERS_DIR) + "model.frag");

    shaderWall = std::make_shared<Shader>(std::string(SHADERS_DIR) + "model.vert",
                                          std::string(SHADERS_DIR) + "model.frag");

    srand(time(NULL));

    shaderCube->Bind();
    
    game = new Blockout();
    game->Init();

    scene = new Scene();
    scene->Generate();
    
    shaderCube->Unbind();
    
    camera = PerspectiveCamera();
    camera.SetFrustrum(PerspectiveCamera::Frustrum(45.0f, width, height, 0.1f, 100.0f));


    TextureManager::GetInstance()->LoadTexture2DRGBA("cube_texture",
                                                     std::string(TEXTURES_DIR) + "cube_texture.png",
                                                     0, true);

    TextureManager::GetInstance()->LoadTexture2DRGBA("floor_texture",
                                                     std::string(TEXTURES_DIR) + "marble.jpg",
                                                     1, false);


    shaderCube->Bind();
    shaderCube->UploadUniformInt("mode", 3);
    shaderCube->Unbind();

    shaderWall->Bind();
    shaderWall->UploadUniformInt("mode", 4);
    shaderWall->Unbind();


    return 1;
}

unsigned Application::Run()
{
    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        currentFrame = static_cast<float>(glfwGetTime());
        currentTime = currentFrame - startFrame;

        Input(window);
        game->Update(currentTime, startFrame);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        glm::mat4 model = glm::mat4(1.0f);
        const glm::mat4 projection = camera.GetProjectionMatrix();
        const glm::mat4 view = camera.GetViewMatrix();
        model = glm::translate(model, glm::vec3(-2.5f, 0.0f, -2.5f));
        model = glm::scale(model, glm::vec3(5.0f));

        shaderCube->Bind();


        shaderCube->UploadUniformVec3("objectColor", 1.0f, 0.5f, 0.31f);
        shaderCube->UploadUniformVec3("lightColor", 1.0f, 1.0f, 1.0f);
        shaderCube->UploadUniformVec3("lightPos", lightPos);
        shaderCube->UploadUniformVec3("viewPos", camera.GetPosition());


        shaderCube->UploadUniformVec4("color", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
        shaderCube->UploadUniformMat4("projection", projection);
        shaderCube->UploadUniformMat4("view", view);
        shaderCube->UploadUniformMat4("model", model);


        shaderCube->Unbind();

        game->Render(*shaderCube, projection, view);
        scene->Render(*shaderWall, projection, view);

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
            // isBinding = !isBinding;
            // shader->UploadUniformBool("isBinding", isBinding);
        }
    };
}

void Application::Input(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
