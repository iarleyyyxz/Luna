// Application.cpp
#include "Application.hpp"
#include <iostream>
#include <memory>

#include "Source/Renderer/Texture.hpp"
#include "Source/Renderer/Sprite.hpp"

#include <glm.hpp>
#include <matrix_transform.hpp>

#include "SceneObject.hpp"
#include "Source/Ecs/Component.hpp"
#include "Source/Ecs/Transform2D.hpp"
#include "Camera2D.hpp" // Include do header da Camera2D
#include <GL/stb_image.h>
#include "Source/Scene/SceneManager.hpp"
#include "Source/Scene/Scene.hpp"
#include "Source/Ecs/SpriteRenderer.hpp"
#include "Source/Ecs/Anim/Animation.hpp"

#define ICON_FA_FOLDER        "\xef\x81\xbb"
#define ICON_FA_FILE          "\xef\x85\x9c"
#define ICON_FA_FILE_CODE     "\xef\x87\x9a"
#define ICON_FA_CHECK         "\xef\x81\xae"
#define ICON_BOX              "\xef\x91\xa6"
// including icon fonts
// ... other includes ...

// Inicialização dos membros estáticos
ImFont* fontIcons; // fonte com ícones
const float Application::GRID_SPACING = 25.0f;
const glm::vec4 Application::GRID_COLOR = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);

void Application::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (app && button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            app->m_isDragging = true;
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            app->m_dragStartPosition = glm::vec2(xpos, ypos);
        }
        else if (action == GLFW_RELEASE) {
            app->m_isDragging = false;
        }
    }
}

void Application::processMousePan(const glm::vec2& currentMousePosition) {
    if (m_isDragging) {
        glm::vec2 delta = currentMousePosition - m_dragStartPosition;
        float panSpeed = 0.5f; // Ajuste a sensibilidade do pan
        m_camera.setPosition(m_camera.getPosition() - delta * panSpeed);
        m_dragStartPosition = currentMousePosition; // Atualiza para o próximo movimento
    }
}

void Application::glfwErrorCallback(int error, const char* description) {
    std::cerr << "Erro GLFW: " << description << std::endl;
}

void Application::windowCloseCallback(GLFWwindow* window) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

Application::Application() : isRunning(true), window(nullptr), screenWidth(800.0f), screenHeight(600.0f),
framebufferTexture(0), framebufferObject(0), m_viewportGui(), m_testSprite(m_spriteTexture.get()),
m_camera(screenWidth, screenHeight)
{
    std::cout << "Application construída!" << std::endl;
}

Application::~Application()
{
    std::cout << "Application destruída!" << std::endl;
    glDeleteFramebuffers(1, &framebufferObject);
    glDeleteTextures(1, &framebufferTexture);
}

bool Application::Init()
{
    std::cout << "Inicializando a Application..." << std::endl;

    glfwSetErrorCallback(Application::glfwErrorCallback);
    if (!glfwInit()) {
        std::cerr << "Falha ao inicializar o GLFW" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(static_cast<int>(screenWidth), static_cast<int>(screenHeight), "Luna Engine Editor", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Falha ao criar a janela do GLFW" << std::endl;
        glfwTerminate();
        return false;
    }

    // Carregar imagem do ícone
    int width, height, channels;
    unsigned char* iconPixels = stbi_load("Resources/Luna Logo.png", &width, &height, &channels, 4);
    if (!iconPixels) {
        printf("Falha ao carregar o ícone\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    GLFWimage icon;
    icon.width = width;
    icon.height = height;
    icon.pixels = iconPixels;

    // Setar ícone da janela
    glfwSetWindowIcon(window, 1, &icon);

    // Liberar dados da imagem após setar o ícone
    stbi_image_free(iconPixels);


    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetKeyCallback(window, [](GLFWwindow* win, int key, int scancode, int action, int mods) {
        auto app = static_cast<Application*>(glfwGetWindowUserPointer(win));
        if (app) app->m_keyboard.UpdateKeyState(key, action);
        });

    glfwSetCursorPosCallback(window, [](GLFWwindow* win, double xpos, double ypos) {
        auto app = static_cast<Application*>(glfwGetWindowUserPointer(win));
        if (app) app->m_mouse.ProcessCursorPosition(xpos, ypos);
        });

    glfwSetMouseButtonCallback(window, [](GLFWwindow* win, int button, int action, int mods) {
        auto app = static_cast<Application*>(glfwGetWindowUserPointer(win));
        if (app) app->m_mouse.ProcessMouseButton(button, action);
        }); 
    glfwSetMouseButtonCallback(window, Application::mouseButtonCallback);

    glfwSetScrollCallback(window, [](GLFWwindow* win, double xoffset, double yoffset) {
        auto app = static_cast<Application*>(glfwGetWindowUserPointer(win));
        if (app) app->m_mouse.ProcessScroll(xoffset, yoffset);
        });

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Falha ao inicializar o GLEW" << std::endl;
        glfwTerminate();
        return false;
    }

    glViewport(0, 0, static_cast<int>(screenWidth), static_cast<int>(screenHeight));
    glfwSetWindowCloseCallback(window, Application::windowCloseCallback);
    glfwSetWindowUserPointer(window, this);

    if (!m_renderer2D.Init(screenWidth, screenHeight)) {
        std::cerr << "Falha ao inicializar o Renderer2D." << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return false;
    }

    if (!InitFramebuffer()) {
        std::cerr << "Falha ao inicializar o Framebuffer." << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return false;
    }

    if (!m_imGuiManager.Init(window, "#version 330 core", m_renderer2D)) {
        std::cerr << "Falha ao inicializar o ImGuiManager." << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return false;
    }

    // Obtenha a instância do SceneManager
   

    // Crie e carregue sua primeira cena
    sceneManager.LoadScene("Cena Padrão");
    std::shared_ptr<Scene> currentScene = sceneManager.GetCurrentScene();

    if (currentScene) {
        // Crie um SceneObject
        std::shared_ptr<SceneObject> player = std::make_shared<SceneObject>("Player");
        player->GetTransform().position = glm::vec2(100.0f, 100.0f);
        player->GetTransform().scale = glm::vec3(50.0f, 50.0f, 1.0f);

        // Crie e adicione um SpriteRenderer ao player
        // Supondo que você tenha uma forma de carregar Textures
        std::shared_ptr<Texture> playerTexture = std::make_shared<Texture>("Resources/textura.png");
        // player->AddComponent(std::make_shared<SpriteRenderer>(playerTexture));
        player->AddComponent(std::make_shared<SpriteRenderer>(playerTexture, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f))); // Exemplo: sprite vermelho
        
        // Adicione o player à cena
        currentScene->AddSceneObject(player);

        // Crie um objeto filho para o player
        std::shared_ptr<SceneObject> childObject = std::make_shared<SceneObject>("ChildOfPlayer");
        childObject->GetTransform().position = glm::vec2(20.0f, 20.0f); // Posição relativa ao pai
        childObject->GetTransform().scale = glm::vec3(20.0f, 20.0f, 1.0f);
        childObject->AddComponent(std::make_shared<SpriteRenderer>(nullptr, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f))); // Exemplo: sprite azul
        player->AddChild(childObject);

        // 2
        std::shared_ptr<SceneObject> pl2 = std::make_shared<SceneObject>("Player 2");
        pl2->GetTransform().position = glm::vec2(100.0f, 100.0f);
        pl2->GetTransform().scale = glm::vec3(50.0f, 50.0f, 1.0f);

        // Crie e adicione um SpriteRenderer ao player
        // Supondo que você tenha uma forma de carregar Textures
        std::shared_ptr<Texture> pl = std::make_shared<Texture>("Resources/example.png");
        // player->AddComponent(std::make_shared<SpriteRenderer>(playerTexture));
        pl2->AddComponent(std::make_shared<SpriteRenderer>(pl, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f))); // Exemplo: sprite vermelho

        // Adicione o player à cena
        currentScene->AddSceneObject(pl2);

        // Crie um objeto filho para o player
        std::shared_ptr<SceneObject> objc = std::make_shared<SceneObject>("Child");
        objc->GetTransform().position = glm::vec2(20.0f, 20.0f); // Posição relativa ao pai
        objc->GetTransform().scale = glm::vec3(20.0f, 20.0f, 1.0f);
        objc->AddComponent(std::make_shared<SpriteRenderer>(nullptr, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f))); // Exemplo: sprite azul
        pl2->AddChild(objc);
    }

    if (!m_imGuiManager.LoadFont("Resources/Fonts/Roboto-Medium.ttf", 18))
    {
        std::cerr << "Fail load font imGui";
    }


    static const ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 };
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    fontIcons = ImGui::GetIO().Fonts->AddFontFromFileTTF("Resources/Fonts/fa-solid-900.ttf", 16.0f, &icons_config, icons_ranges);
  
    // Application.cpp (dentro da função Application::Init(), após carregar a textura)

    // Carregar a textura da sprite (já existe no seu código)
   // Carregar a textura da sprite


    std::cout << "Application inicializada com sucesso." << std::endl;
    return true;
}

bool Application::InitFramebuffer()
{
    glGenFramebuffers(1, &framebufferObject);
    glBindFramebuffer(GL_FRAMEBUFFER, framebufferObject);

    glGenTextures(1, &framebufferTexture);
    glBindTexture(GL_TEXTURE_2D, framebufferTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, static_cast<int>(screenWidth), static_cast<int>(screenHeight), 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture, 0);

    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, static_cast<int>(screenWidth), static_cast<int>(screenHeight));
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Erro: Framebuffer não está completo!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDeleteFramebuffers(1, &framebufferObject);
        glDeleteTextures(1, &framebufferTexture);
        glDeleteRenderbuffers(1, &rbo);
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteRenderbuffers(1, &rbo);

    return true;
}

void Application::Run()
{
    std::cout << "Executando a Application..." << std::endl;

    double lastFrameTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        double currentFrameTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentFrameTime - lastFrameTime);
        lastFrameTime = currentFrameTime;

        m_mouse.UpdatePosition(window);

        m_imGuiManager.BeginFrame();

        // Renderizar a Viewport GUI
        m_viewportGui.Render(framebufferTexture);


        m_imGuiManager.DrawEditorUI(deltaTime);
        sceneManager.OnGui();

        // *** Renderizar para o Framebuffer ***
        glBindFramebuffer(GL_FRAMEBUFFER, framebufferObject);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::vec2 currentMousePos = m_mouse.GetPosition();

        // *** CONTROLES DA CÂMERA (PAN COM O MOUSE) ***
       // processMousePan(currentMousePos);

        if (m_keyboard.IsKeyPressed(GLFW_KEY_EQUAL)) {
            m_camera.setZoom(m_camera.getZoom() + 0.1f);
        }
        if (m_keyboard.IsKeyPressed(GLFW_KEY_MINUS)) {
            m_camera.setZoom(m_camera.getZoom() - 0.1f);
        } 

        // UPDATING CAMERA    
       m_camera.update(deltaTime);

      
       sceneManager.Update(deltaTime);

        // --- START OF SCENE RENDER ---
        glm::mat4 projection = m_camera.getProjectionMatrix() * m_camera.getViewMatrix();
        m_renderer2D.beginScene(projection);
        
        // Renderizar o World
      
       // m_renderer2D.drawQuad(glm::vec2(150, 150), glm::vec2(15.0f, 15.0f), glm::vec4(3.0f, 1.0f, 4.0f, 1.0f));
        sceneManager.Render(m_renderer2D);

        m_renderer2D.endScene();
        // --- END OF SCENE RENDER ---

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        m_imGuiManager.EndFrame(window);

        glfwSwapBuffers(window);
    }
    isRunning = false;
}

glm::vec4 Application::GetTileColor(int tileId)
{
    // Uma função temporária para associar um ID a uma cor
    switch (tileId) {
    case 1: return glm::vec4(0.8f, 0.2f, 0.3f, 1.0f); // Vermelho
    case 2: return glm::vec4(0.2f, 0.7f, 0.4f, 1.0f); // Verde
    case 3: return glm::vec4(0.1f, 0.5f, 0.8f, 1.0f); // Azul
    default: return glm::vec4(1.0f); // Branco (vazio ou desconhecido)
    }
}

void Application::RenderGrid()
{
    float width = screenWidth;
    float height = screenHeight;
    float spacing = GRID_SPACING;

    // Desenhar linhas verticais
    for (float x = 0.0f; x < width; x += spacing)
    {
        m_renderer2D.drawLine(glm::vec2(x, 0.0f), glm::vec2(x, height), GRID_COLOR);
    }

    // Desenhar linhas horizontais
    for (float y = 0.0f; y < height; y += spacing)
    {
        m_renderer2D.drawLine(glm::vec2(0.0f, y), glm::vec2(width, y), GRID_COLOR);
    }
}

void Application::Shutdown()
{
    std::cout << "Encerrando a Application..." << std::endl;

    m_imGuiManager.Shutdown();
    m_renderer2D.Shutdown();

    glDeleteFramebuffers(1, &framebufferObject);
    glDeleteTextures(1, &framebufferTexture);

    if (window) {
        glfwDestroyWindow(window);
    }
    glfwTerminate();

    std::cout << "Application encerrada." << std::endl;
}