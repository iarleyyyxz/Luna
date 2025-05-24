#include "Application.hpp"
#include <iostream>
#include <memory> // Para std::make_unique

// Definições das funções de callback estáticas da classe Application
void Application::glfwErrorCallback(int error, const char* description) {
    std::cerr << "Erro GLFW: " << description << std::endl;
}

void Application::windowCloseCallback(GLFWwindow* window) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}


Application::Application() : isRunning(true), window(nullptr), screenWidth(800.0f), screenHeight(600.0f)
{
    std::cout << "Application construída!" << std::endl;
}

Application::~Application()
{
    std::cout << "Application destruída!" << std::endl;
}

bool Application::Init()
{
    std::cout << "Inicializando a Application..." << std::endl;

    // 1. Inicializar GLFW
    glfwSetErrorCallback(Application::glfwErrorCallback);
    if (!glfwInit()) {
        std::cerr << "Falha ao inicializar o GLFW" << std::endl;
        return false;
    }

    // 2. Configurar a janela do GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // 3. Criar a janela
    window = glfwCreateWindow(static_cast<int>(screenWidth), static_cast<int>(screenHeight), "Luna Engine Editor", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Falha ao criar a janela do GLFW" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Ativar VSync

    // 4. Inicializar o GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Falha ao inicializar o GLEW" << std::endl;
        glfwTerminate();
        return false;
    }

    // 5. Configurar o viewport OpenGL
    glViewport(0, 0, static_cast<int>(screenWidth), static_cast<int>(screenHeight));

    // 6. Definir o callback de fechar janela
    glfwSetWindowCloseCallback(window, Application::windowCloseCallback);

    // 7. Definir o ponteiro do utilizador da janela para 'this' (instância da Application)
    glfwSetWindowUserPointer(window, this);

    // 8. Inicializar o Renderer2D
    if (!m_renderer2D.Init(screenWidth, screenHeight)) {
        std::cerr << "Falha ao inicializar o Renderer2D." << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return false;
    }

    // 9. Inicializar o SceneManager
    // Definir as referências da engine no SceneManager ANTES de inicializar o ImGuiManager
    m_sceneManager.m_windowRef = window;
    m_sceneManager.m_screenWidthRef = screenWidth;
    m_sceneManager.m_screenHeightRef = screenHeight;
    m_sceneManager.m_rendererRef = &m_renderer2D;
    m_sceneManager.m_keyboardRef = &m_keyboard;
    m_sceneManager.m_mouseRef = &m_mouse;

    if (!m_sceneManager.Init()) {
        std::cerr << "Falha ao inicializar o SceneManager." << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return false;
    }

    // 10. Inicializar ImGuiManager, passando as dependências
    // O ImGuiManager agora recebe as referências para os gerenciadores de input e renderização,
    // e também para o SceneManager, para que seus editores possam interagir com eles.
    if (!m_imGuiManager.Init(window, "#version 330 core", m_renderer2D, m_keyboard, m_mouse)) {
        std::cerr << "Falha ao inicializar o ImGuiManager." << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return false;
    }

    // Carregar uma fonte personalizada (ex: "Resources/Fonts/Roboto-Medium.ttf")
    if (!m_imGuiManager.LoadFont("Resources/Fonts/Roboto-Medium.ttf", 16.0f)) {
        std::cerr << "Falha ao carregar a fonte personalizada. Usando a fonte padrão do ImGui." << std::endl;
    }

    // Adicionar uma cena inicial padrão e alternar para ela
    m_sceneManager.addScene("Cena Inicial", std::make_unique<Luna::Scene>("Cena Inicial"));
    m_sceneManager.switchToScene("Cena Inicial", window, screenWidth, screenHeight,
        m_renderer2D, m_keyboard, m_mouse);


    std::cout << "Application inicializada com sucesso. Pronto para o utilizador registar as suas cenas." << std::endl;
    return true;
}

void Application::Run()
{
    std::cout << "Executando a Application..." << std::endl;

    double lastFrameTime = glfwGetTime(); // Para calcular deltaTime no loop principal

    // Loop principal da aplicação
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents(); // Processar eventos GLFW (input, fechamento de janela, etc.)

        // Calcular deltaTime
        double currentFrameTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentFrameTime - lastFrameTime);
        lastFrameTime = currentFrameTime;

        // Atualizar o estado do mouse (delta)
        m_mouse.UpdatePosition(window); // Usar m_mouse

        // Iniciar o novo frame do ImGui
        m_imGuiManager.BeginFrame(); // Usar m_imGuiManager

        // Desenhar a interface do SpritesheetEditor através do ImGuiManager
        m_imGuiManager.DrawEditorUI(deltaTime); // Usar m_imGuiManager

        // Limpar o buffer de cor e profundidade antes de cada frame
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Cor de fundo (azul-esverdeado)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Renderizar a cena atual (se houver uma)
        if (m_sceneManager.getCurrentScene()) {
            m_sceneManager.UpdateCurrentScene(deltaTime);
            m_sceneManager.RenderCurrentScene();
        }

        // Finalizar e renderizar o ImGui
        m_imGuiManager.EndFrame(window); // Usar m_imGuiManager

        // Trocar os buffers da janela para exibir o frame renderizado
        glfwSwapBuffers(window);
    }
    isRunning = false; // Define como false quando a janela deve fechar
}

void Application::Shutdown()
{
    std::cout << "Encerrando a Application..." << std::endl;

    // Desligar ImGuiManager
    m_imGuiManager.Shutdown(); // Usar m_imGuiManager

    // Encerrar o SceneManager (isso desligará a cena atual e liberará todas as cenas)
    m_sceneManager.Shutdown(); // Usar m_sceneManager

    // Encerrar o Renderer2D
    m_renderer2D.Shutdown(); // Usar m_renderer2D

    // Destruir a janela GLFW
    if (window) {
        glfwDestroyWindow(window);
    }
    // Terminar o GLFW
    glfwTerminate();

    std::cout << "Application encerrada." << std::endl;
}