#include "Application.hpp"
#include <iostream>
#include <memory> // Para std::make_unique

// Definições das funcoes de callback estáticas da classe Application
void Application::glfwErrorCallback(int error, const char* description) {
    std::cerr << "Erro GLFW: " << description << std::endl;
}

void Application::windowCloseCallback(GLFWwindow* window) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}


Application::Application() : isRunning(true), window(nullptr), screenWidth(800.0f), screenHeight(600.0f), sceneManager() // Inicializa sceneManager aqui
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
    if (!renderer2D.Init(screenWidth, screenHeight)) {
        std::cerr << "Falha ao inicializar o Renderer2D." << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return false;
    }

    // 9. Inicializar o SceneManager
    if (!sceneManager.Init()) {
        std::cerr << "Falha ao inicializar o SceneManager." << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return false;
    }
    // Configurar Renderer2D e dimensões da tela no SceneManager
  //  sceneManager->setRenderer(m_renderer);
   // sceneManager->setScreenSize(m_viewportWidth, m_viewportHeight);
    sceneManager.setRenderer(&renderer2D);
    sceneManager.setScreenSize(screenWidth, screenHeight);
    // Carregar uma cena inicial (opcional)
    sceneManager.addScene("default");
    sceneManager.loadScene("default");

    // Inicializar ImGuiManager, passando as dependências do SpritesheetEditor
    if (!imGuiManager.Init(window, "#version 330 core", renderer2D, keyboard, mouse)) {
        std::cerr << "Falha ao inicializar o ImGuiManager." << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return false;
    }

    // Carregar uma fonte personalizada
    if (!imGuiManager.LoadFont("Resources/Fonts/Roboto-Medium.ttf", 16.0f)) {
        std::cerr << "Falha ao carregar a fonte personalizada. Usando a fonte padrão do ImGui." << std::endl;
    }

    std::cout << "Application inicializada com sucesso. Pronto para o utilizador registar as suas cenas." << std::endl;
    return true;
}

void Application::Run()
{
    std::cout << "Executando a Application..." << std::endl;

    double lastFrameTime = glfwGetTime(); // Para calcular deltaTime no loop principal

    // Loop principal da aplicação
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents(); // Processar eventos GLFW

        // Calcular deltaTime
        double currentFrameTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentFrameTime - lastFrameTime);
        lastFrameTime = currentFrameTime;

        // Atualizar o estado do mouse
        mouse.UpdatePosition(window);

        // Iniciar o novo frame do ImGui
        imGuiManager.BeginFrame();

        // Desenhar a interface do Editor (incluindo o SceneManager)
        imGuiManager.DrawEditorUI(deltaTime);
        sceneManager.OnImGuiRender(); // Renderizar a interface do SceneManager

        // Limpar o buffer de cor e profundidade
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Renderizar a cena atual
        sceneManager.RenderCurrentScene();

        // Finalizar e renderizar o ImGui
        imGuiManager.EndFrame(window);

        // Trocar os buffers da janela
        glfwSwapBuffers(window);
    }
    isRunning = false; // Define como false quando a janela deve fechar
}

void Application::Shutdown()
{
    std::cout << "Encerrando a Application..." << std::endl;

    // Desligar ImGuiManager
    imGuiManager.Shutdown();

    // Encerrar o SceneManager
    sceneManager.Shutdown();

    // Encerrar o Renderer2D
    renderer2D.Shutdown();

    // Destruir a janela GLFW
    if (window) {
        glfwDestroyWindow(window);
    }
    // Terminar o GLFW
    glfwTerminate();

    std::cout << "Application encerrada." << std::endl;
}