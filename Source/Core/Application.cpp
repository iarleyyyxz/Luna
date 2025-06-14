// Application.cpp
#include "Application.hpp"
#include <iostream>
#include <memory>

#include "Texture.hpp"
#include "Sprite.hpp"

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
#include <type_ptr.hpp>

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

// Métodos estáticos de callback (precisam do ponteiro de usuário para acessar a instância da classe)
void Application::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (app) {
        app->m_mouse.ProcessMouseButton(button, action, mods);
    }
}

void Application::CursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (app) {
        app->m_mouse.ProcessCursorPosition(xpos, ypos);
    }
}

void Application::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (app) {
        app->m_mouse.ProcessScroll(xoffset, yoffset);
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
m_camera(screenWidth, screenHeight), m_selectedObjectForGizmo(nullptr)
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
        if (app) app->m_mouse.ProcessMouseButton(button, action, mods);
        }); 
    glfwSetMouseButtonCallback(window, Application::MouseButtonCallback);

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

    // --- Configuração da Cena e Objetos de Teste ---
    sceneManager.LoadScene("Cena Padrão");
    std::shared_ptr<Scene> currentScene = sceneManager.GetCurrentScene();

    if (currentScene) {
        // Objeto principal para o Gizmo
        m_selectedObjectForGizmo = std::make_shared<SceneObject>("PlayerGizmo");
        m_selectedObjectForGizmo->GetTransform().position = glm::vec2(200.0f, 200.0f);
        m_selectedObjectForGizmo->GetTransform().scale = glm::vec3(100.0f, 100.0f, 1.0f);
        // Carregue uma textura real para o sprite do objeto de teste
        // Use AssetManager para carregar texturas
        std::shared_ptr<Texture> gizmoSelectedTexture = std::make_shared<Texture>("Resources/example.png");
        m_selectedObjectForGizmo->AddComponent(std::make_shared<SpriteRenderer>(gizmoSelectedTexture, glm::vec4(1.0f)));
        m_selectedObjectForGizmo->AddComponent(std::make_shared <Luna::Ecs::Anim::Animation>(nullptr, nullptr, 1.0f));
        currentScene->AddSceneObject(m_selectedObjectForGizmo);

        // Objeto secundário (sem gizmo)
        std::shared_ptr<SceneObject> player2 = std::make_shared<SceneObject>("Player2");
        player2->GetTransform().position = glm::vec2(400.0f, 150.0f);
        player2->GetTransform().scale = glm::vec3(80.0f, 80.0f, 1.0f);
        std::shared_ptr<Texture> player2Texture =
           std::make_shared<Texture>("Resources/example.png"); // *** VERIFIQUE ESTE CAMINHO ***
        player2->AddComponent(std::make_shared<SpriteRenderer>(player2Texture, glm::vec4(1.0f, 0.5f, 0.0f, 1.0f)));
        currentScene->AddSceneObject(player2);
    }

    // *** Inicializa o Gizmo 2D com o objeto selecionado ***
    // Certifique-se de que m_selectedObjectForGizmo está inicializado antes de passar.
    m_gizmo.SetSelectedObject(m_selectedObjectForGizmo.get());


    if (!m_imGuiManager.LoadFont("Resources/Fonts/Roboto-Medium.ttf", 18))
    {
        std::cerr << "Fail load font imGui";
    }

    // Adding icon fonts
    static const ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 };
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    fontIcons = ImGui::GetIO().Fonts->AddFontFromFileTTF("Resources/Fonts/fa-solid-900.ttf", 16.0f, &icons_config, icons_ranges);

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
        deltaTime = static_cast<float>(currentFrameTime - lastFrameTime);
        lastFrameTime = currentFrameTime;

        ProcessInput(window); // Atualiza m_keyboard e m_mouse

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

       m_gizmo.Update(m_mouse.GetPosition().x, m_mouse.GetPosition().y, m_keyboard.IsKeyPressed(GLFW_KEY_H), m_camera);
      
       sceneManager.Update(deltaTime);

        // --- START OF SCENE RENDER ---
        glm::mat4 projection = m_camera.getProjectionMatrix() * m_camera.getViewMatrix();
        m_renderer2D.beginScene(projection);
        
        // Renderizar o World
      
       // m_renderer2D.drawQuad(glm::vec2(150, 150), glm::vec2(15.0f, 15.0f), glm::vec4(3.0f, 1.0f, 4.0f, 1.0f));
        sceneManager.Render(m_renderer2D);

        m_gizmo.Render(m_renderer2D, m_camera); // Renderiza o gizmo por cima da cena

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

void Application::ProcessInput(GLFWwindow* wwindow) {
    // A m_keyboard e m_mouse são atualizadas pelos callbacks GLFW.
    // Você pode adicionar lógica de input adicional aqui se precisar de processamento contínuo.
    // Exemplo: Mover a câmera com WASD
    m_mouse.UpdatePosition(wwindow);

    // Lógica para arrastar o gizmo
    glm::vec2 mousePos = m_mouse.GetPosition();
    glm::vec2 gizmoPos = m_gizmo.GetPosition(); // Pega a posição atual do gizmo
    float gizmoRadius = m_gizmo.GetRadius();    // Assumindo que o gizmo tem um raio/tamanho para detecção de clique

    // Verifica se o mouse está sobre o gizmo
    bool isMouseOverGizmo = glm::distance(mousePos, gizmoPos) < gizmoRadius;

    // Se o botão esquerdo do mouse foi pressionado e o mouse está sobre o gizmo
    if (m_mouse.IsButtonPressed(GLFW_MOUSE_BUTTON_LEFT) && isMouseOverGizmo && !m_isGizmoDragging) {
        m_isGizmoDragging = true;
        // Calcula o offset inicial entre o mouse e o centro do gizmo
        m_dragOffset = mousePos - gizmoPos;
        // Poderíamos também registrar a posição inicial do gizmo aqui se o cálculo de offset fosse diferente
    }
    // Se o botão esquerdo do mouse está pressionado E estamos arrastando
    else if (m_keyboard.IsKeyPressed(GLFW_KEY_G) && m_isGizmoDragging) {
        // Nova posição do gizmo é a posição atual do mouse menos o offset inicial
        Luna::GLogger.Info("Mouse pressed");
        m_gizmo.SetPosition(mousePos - m_dragOffset);
    }
    // Se o botão esquerdo do mouse foi liberado E estávamos arrastando
    else if (!m_mouse.IsButtonPressed(GLFW_MOUSE_BUTTON_LEFT) && m_isGizmoDragging) {
        m_isGizmoDragging = false; // Para de arrastar
    }

    // Limpa o scroll no final do frame se não quiser que seja cumulativo
    m_mouse.ClearScroll();
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