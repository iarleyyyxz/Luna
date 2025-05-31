// Application.cpp
#include "Application.hpp"
#include <iostream>
#include <memory>

#include "Source/Renderer/Texture.hpp"
#include "Source/Renderer/Sprite.hpp"

#include <glm.hpp>
#include <matrix_transform.hpp>

// Inicialização dos membros estáticos
const float Application::GRID_SPACING = 25.0f;
const glm::vec4 Application::GRID_COLOR = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);

void Application::glfwErrorCallback(int error, const char* description) {
    std::cerr << "Erro GLFW: " << description << std::endl;
}

void Application::windowCloseCallback(GLFWwindow* window) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

Application::Application() : isRunning(true), window(nullptr), screenWidth(800.0f), screenHeight(600.0f),
framebufferTexture(0), framebufferObject(0), m_viewportGui(), m_testSprite(m_spriteTexture.get())
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
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

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

    if (!m_imGuiManager.Init(window, "#version 330 core", m_renderer2D, m_keyboard, m_mouse)) {
        std::cerr << "Falha ao inicializar o ImGuiManager." << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return false;
    }


    if (!m_imGuiManager.LoadFont("Resources/Fonts/Roboto-Medium.ttf", 18))
    {
        std::cerr << "Fail load font imGui";
    }

    if (!m_imGuiManager.LoadFont("Resources/Fonts/fontawesome-webfont.ttf", 16))
    {
        std::cerr << "Fail load font imGui";
    }

    // Carregar a textura da sprite
    m_spriteTexture = std::make_unique<Texture>("Resources/textura.png");
    if (m_spriteTexture->getID() == 0) {
        std::cerr << "Falha ao carregar a textura da sprite." << std::endl;
        // Não retornar false aqui, pois a aplicação ainda pode funcionar sem esta sprite.
    }
    m_testSprite = Sprite(m_spriteTexture.get());

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

        // *** Renderizar para o Framebuffer ***
        glBindFramebuffer(GL_FRAMEBUFFER, framebufferObject);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // --- START OF SCENE RENDER ---
        glm::mat4 projection = glm::ortho(0.0f, screenWidth, screenHeight, 0.0f, -1.0f, 1.0f);
        m_renderer2D.beginScene(projection);

        // Renderizar a grade
        RenderGrid();

        // Renderizar a sprite carregada do arquivo
        m_renderer2D.drawSprite(glm::vec2(100.0f, 100.0f), glm::vec2(100.0f, 100.0f), m_testSprite, glm::vec4(1.0f));

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