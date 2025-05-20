#include "Game.hpp"
#include <glm.hpp>
#include <matrix_transform.hpp>
#include <iostream>
// #include "Texture.hpp" // Removido: Não é mais necessário aqui

// Definições das funções de callback estáticas da classe Game
// Elas usam glfwGetWindowUserPointer para acessar a instância da Game.
void Game::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
    if (game) {
        game->m_keyboard.UpdateKeyState(key, action);
    }
}

void Game::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
    Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
    if (game) {
        game->m_mouse.X = xpos;
        game->m_mouse.Y = ypos;
    }
}

void Game::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
    if (game) {
        game->m_mouse.UpdateButtonState(button, action);
    }
}

void Game::windowCloseCallback(GLFWwindow* window) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}


Game::Game() : m_window(nullptr), m_screenWidth(0.0f), m_screenHeight(0.0f) // Removido m_exampleTexture(nullptr)
{
    std::cout << "Jogo construído!" << std::endl;
}

Game::~Game() {
    Shutdown();
    std::cout << "Jogo destruído!" << std::endl;
}

bool Game::Init(GLFWwindow* window, float screenWidth, float screenHeight) {
    m_window = window;
    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;

    // Registrar as funções de callback do GLFW com a instância da Game
    glfwSetKeyCallback(m_window, Game::keyCallback);
    glfwSetCursorPosCallback(m_window, Game::cursorPositionCallback);
    glfwSetMouseButtonCallback(m_window, Game::mouseButtonCallback);
    glfwSetWindowCloseCallback(m_window, Game::windowCloseCallback);
    glfwSetWindowUserPointer(m_window, this); // Definir o ponteiro do usuário da janela para 'this'

    // Inicializar o Renderer2D com as dimensões da tela
    if (!m_renderer2D.Init(m_screenWidth, m_screenHeight)) {
        std::cerr << "Falha ao inicializar o Renderer2D." << std::endl;
        return false;
    }

    // Código de carregamento da textura de exemplo removido
    // m_exampleTexture = new Texture("Resources/textura.jpeg");
    // if (!m_exampleTexture->getID()) {
    //     std::cerr << "Falha ao carregar a textura de exemplo. Certifique-se de que o caminho está correto." << std::endl;
    //     delete m_exampleTexture;
    //     m_exampleTexture = nullptr;
    //     // Não retorna false aqui se a textura for opcional e o jogo puder continuar sem ela
    // }

    std::cout << "Jogo inicializado com sucesso." << std::endl;
    return true;
}

void Game::Run() {
    // Processar eventos GLFW (agora dentro do Game)
    glfwPollEvents();

    // Processar input da Game
    processInput();

    // Lógica de atualização e renderização
    Update(0.0f); // Placeholder para deltaTime
    Render();
}

void Game::Update(float deltaTime) {
    // Lógica de atualização do jogo
    // Exemplo: mover entidades, calcular física, etc.
}

void Game::Render() {
    // A preparação do frame (limpeza de buffer) é feita pela Application.

    // Iniciar a cena 2D com a matriz de projeção ortográfica
    glm::mat4 projection = glm::ortho(0.0f, m_screenWidth, m_screenHeight, 0.0f, -1.0f, 1.0f);
    m_renderer2D.beginScene(projection);

    // Desenhar quads com cores sólidas
    if (this->m_keyboard.IsKeyJustPressed(GLFW_KEY_T))
    {
        m_renderer2D.drawQuad(glm::vec2(100.0f, 100.0f), glm::vec2(50.0f, 50.0f), glm::vec4(1.0f, 0.5f, 0.2f, 1.0f));
    }
    else if (this->m_keyboard.IsKeyJustPressed(GLFW_KEY_R))
    {
        m_renderer2D.drawQuad(glm::vec2(400.0f, 100.0f), glm::vec2(100.0f, 55.0f), glm::vec4(0.8f, 0.2f, 0.9f, 1.0f));
    }
    // Código de desenho da textura de exemplo removido
    // if (m_exampleTexture && m_exampleTexture->getID()) {
    //     m_renderer2D.drawQuad(glm::vec2(200.0f, 150.0f), glm::vec2(100.0f, 100.0f), *m_exampleTexture);
    // }

    // Terminar a cena 2D
    m_renderer2D.endScene();
}

void Game::Shutdown() {
    // Encerrar o Renderer2D
    m_renderer2D.Shutdown();

    // Código de liberação da textura de exemplo removido
    // if (m_exampleTexture) {
    //     delete m_exampleTexture;
    //     m_exampleTexture = nullptr;
    // }

    std::cout << "Recursos de jogo encerrados." << std::endl;
}

void Game::processInput() {
    // Atualizar o estado do mouse
    m_mouse.UpdatePosition(m_window);

   
}
