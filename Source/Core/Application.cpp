#include "Application.hpp"
#include <iostream>

// Definições das funções de callback estáticas da classe Application
// A maioria dos callbacks de input foi movida para a classe Game.
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
    window = glfwCreateWindow(static_cast<int>(screenWidth), static_cast<int>(screenHeight), "Luna Engine", nullptr, nullptr);
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

    // 6. Definir o callback de fechar janela (opcional, se a Application ainda precisar dele)
    glfwSetWindowCloseCallback(window, Application::windowCloseCallback);
    // Removido: glfwSetKeyCallback, glfwSetCursorPosCallback, glfwSetMouseButtonCallback
    // Removido: glfwSetWindowUserPointer (Game fará isso)

    // 7. Inicializar a instância da classe Game, passando o ponteiro da janela e suas dimensões
    if (!game.Init(window, screenWidth, screenHeight)) {
        std::cerr << "Falha ao inicializar o jogo dentro da Application." << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return false;
    }

    std::cout << "Application inicializada com sucesso." << std::endl;
    return true;
}

void Application::Run()
{
    std::cout << "Executando a Application..." << std::endl;

    // Loop principal da aplicação
    while (!glfwWindowShouldClose(window)) {
        // Removido: glfwPollEvents() (agora no Game::Run())
        // Removido: mouse.updatePosition(window) (agora no Game::processInput())

        // Limpar o buffer de cor e profundidade antes de cada frame
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Chamar o método Run da instância da Game para processar input, atualizar e renderizar
        game.Run();

        // Trocar os buffers da janela para exibir o frame renderizado
        glfwSwapBuffers(window);
    }
    isRunning = false;
}

void Application::Shutdown()
{
    std::cout << "Encerrando a Application..." << std::endl;

    // 1. Encerrar a instância da Game
    game.Shutdown();

    // 2. Destruir a janela GLFW
    if (window) {
        glfwDestroyWindow(window);
    }
    // 3. Terminar o GLFW
    glfwTerminate();

    std::cout << "Application encerrada." << std::endl;
}
