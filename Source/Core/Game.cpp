#include "Game.hpp"
#include <glm.hpp>
#include <matrix_transform.hpp>
#include <iostream>
// #include "Texture.hpp" // Removido: N�o � mais necess�rio aqui

// Defini��es das fun��es de callback est�ticas da classe Game
// Elas usam glfwGetWindowUserPointer para acessar a inst�ncia da Game.
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
    std::cout << "Jogo constru�do!" << std::endl;
}

Game::~Game() {
    Shutdown();
    std::cout << "Jogo destru�do!" << std::endl;
}

bool Game::Init(GLFWwindow* window, float screenWidth, float screenHeight) {
    m_window = window;
    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;

    // Registrar as fun��es de callback do GLFW com a inst�ncia da Game
    glfwSetKeyCallback(m_window, Game::keyCallback);
    glfwSetCursorPosCallback(m_window, Game::cursorPositionCallback);
    glfwSetMouseButtonCallback(m_window, Game::mouseButtonCallback);
    glfwSetWindowCloseCallback(m_window, Game::windowCloseCallback);
    glfwSetWindowUserPointer(m_window, this); // Definir o ponteiro do usu�rio da janela para 'this'

    // Inicializar o Renderer2D com as dimens�es da tela
    if (!m_renderer2D.Init(m_screenWidth, m_screenHeight)) {
        std::cerr << "Falha ao inicializar o Renderer2D." << std::endl;
        return false;
    }

    // C�digo de carregamento da textura de exemplo removido
    // m_exampleTexture = new Texture("Resources/textura.jpeg");
    // if (!m_exampleTexture->getID()) {
    //     std::cerr << "Falha ao carregar a textura de exemplo. Certifique-se de que o caminho est� correto." << std::endl;
    //     delete m_exampleTexture;
    //     m_exampleTexture = nullptr;
    //     // N�o retorna false aqui se a textura for opcional e o jogo puder continuar sem ela
    // }

    std::cout << "Jogo inicializado com sucesso." << std::endl;
    return true;
}

void Game::Run() {
    // Processar eventos GLFW (agora dentro do Game)
    glfwPollEvents();

    // Processar input da Game
    processInput();

    // L�gica de atualiza��o e renderiza��o
    Update(0.0f); // Placeholder para deltaTime
    Render();
}

void Game::Update(float deltaTime) {
    // L�gica de atualiza��o do jogo
    // Exemplo: mover entidades, calcular f�sica, etc.
}

void Game::Render() {
    // A prepara��o do frame (limpeza de buffer) � feita pela Application.

    // Iniciar a cena 2D com a matriz de proje��o ortogr�fica
    glm::mat4 projection = glm::ortho(0.0f, m_screenWidth, m_screenHeight, 0.0f, -1.0f, 1.0f);
    m_renderer2D.beginScene(projection);

    // Desenhar quads com cores s�lidas
    if (this->m_keyboard.IsKeyJustPressed(GLFW_KEY_T))
    {
        m_renderer2D.drawQuad(glm::vec2(100.0f, 100.0f), glm::vec2(50.0f, 50.0f), glm::vec4(1.0f, 0.5f, 0.2f, 1.0f));
    }
    else if (this->m_keyboard.IsKeyJustPressed(GLFW_KEY_R))
    {
        m_renderer2D.drawQuad(glm::vec2(400.0f, 100.0f), glm::vec2(100.0f, 55.0f), glm::vec4(0.8f, 0.2f, 0.9f, 1.0f));
    }
    // C�digo de desenho da textura de exemplo removido
    // if (m_exampleTexture && m_exampleTexture->getID()) {
    //     m_renderer2D.drawQuad(glm::vec2(200.0f, 150.0f), glm::vec2(100.0f, 100.0f), *m_exampleTexture);
    // }

    // Terminar a cena 2D
    m_renderer2D.endScene();
}

void Game::Shutdown() {
    // Encerrar o Renderer2D
    m_renderer2D.Shutdown();

    // C�digo de libera��o da textura de exemplo removido
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
