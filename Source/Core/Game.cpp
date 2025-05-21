#include "Game.hpp"
#include <glm.hpp>
#include <matrix_transform.hpp>
#include <iostream>

// Definições das funções de callback estáticas da classe Game
void Game::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
    if (game) {
        game->m_keyboard.UpdateKeyState(key, action);
    }
}

void Game::cursorPositionCallback(GLFWwindow* window, double xpos, double double_ypos) { // Renomeado ypos para double_ypos para evitar conflito
    Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
    if (game) {
        game->m_mouse.X = xpos;
        game->m_mouse.Y = double_ypos; // Usar double_ypos
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


Game::Game() : m_window(nullptr), m_screenWidth(0.0f), m_screenHeight(0.0f), m_mainSpritesheet(nullptr), m_playerAnimation(nullptr), m_lastFrameTime(0.0)
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

    // Inicializar a posição da câmara (ex: no centro da tela)
    m_camera.setPosition(glm::vec2(m_screenWidth / 2.0f, m_screenHeight / 2.0f));
    m_camera.setZoom(1.0f); // Zoom padrão

    // Carregar a spritesheet.
    // Substitua "Resources/spritesheet_example.png" pelo caminho da sua imagem de spritesheet.
    // E ajuste a largura e altura dos frames (ex: 32x32 pixels por frame).
   

    // Inicializar o tempo do último frame para calcular o deltaTime
  //  m_lastFrameTime = glfwGetTime();

    std::cout << "Jogo inicializado com sucesso." << std::endl;
    return true;
}

void Game::Run() {
    // Processar eventos GLFW (agora dentro do Game)
    glfwPollEvents();

    // Calcular deltaTime
    double currentFrameTime = glfwGetTime();
    float deltaTime = static_cast<float>(currentFrameTime - m_lastFrameTime);
    m_lastFrameTime = currentFrameTime;

    // Processar input da Game
    processInput();

    // Lógica de atualização e renderização
    Update(deltaTime); // Passar o deltaTime real
    Render();
}

void Game::Update(float deltaTime) {
    // Lógica de atualização do jogo
    // Exemplo: mover entidades, calcular física, etc.

    // Exemplo de controle de câmara com input
    
    // NOVO: Atualizar a animação do jogador
   
}

void Game::Render() {
    // A preparação do frame (limpeza de buffer) é feita pela Application.

    // Obter a matriz de visualização da câmara
    glm::mat4 viewMatrix = m_camera.getViewMatrix();

    // Iniciar a cena 2D combinando a projeção e a visualização
    glm::mat4 projection = glm::ortho(0.0f, m_screenWidth, m_screenHeight, 0.0f, -1.0f, 1.0f);
    //glm::mat4 combinedMatrix = projection * viewMatrix; // Ordem importa: projeção * view

    m_renderer2D.beginScene(projection); // Passar a matriz combinada

    // Desenhar quads com cores sólidas
    m_renderer2D.drawQuad(glm::vec2(100.0f, 100.0f), glm::vec2(50.0f, 50.0f), glm::vec4(1.0f, 0.5f, 0.2f, 1.0f));
    m_renderer2D.drawQuad(glm::vec2(400.0f, 100.0f), glm::vec2(100.0f, 55.0f), glm::vec4(0.8f, 0.2f, 0.9f, 1.0f));
    m_renderer2D.drawQuad(glm::vec2(200.0f, 200.0f), glm::vec2(120.0f, 120.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)); // Exemplo de um novo quad

    // NOVO: Desenhar o frame atual da animação do jogador

    // Terminar a cena 2D
    m_renderer2D.endScene();
}

void Game::Shutdown() {
    // Encerrar o Renderer2D
    m_renderer2D.Shutdown();

    // NOVO: Liberar a animação e a spritesheet
    // Os Sprites em m_playerWalkFrames são de propriedade da Spritesheet,
    // então não os deletamos individualmente aqui.


    std::cout << "Recursos de jogo encerrados." << std::endl;
}

void Game::processInput() {
    // Atualizar o estado do mouse
    m_mouse.UpdatePosition(m_window);

    // Exemplo de uso do input
   
}
