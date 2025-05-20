#ifndef GAME_HPP
#define GAME_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h> // Necessário para GLFWwindow* e callbacks
#include <iostream>
#include <vector>
#include <string>

#include "Renderer2D.hpp"
#include "InputManager.hpp" // Para Keyboard e Mouse
// #include "Texture.hpp"      // Removido: Não é mais necessário se a Game não gerencia texturas

class Game {
public:
    // Construtor padrão
    Game();
    // Destrutor
    ~Game();

    // Inicialização do jogo
    // Agora recebe o ponteiro da janela GLFW e suas dimensões
    bool Init(GLFWwindow* window, float screenWidth, float screenHeight);
    // Método principal de execução do jogo para um frame (chamado pela Application)
    void Run();
    // Lógica de atualização do jogo
    void Update(float deltaTime);
    // Lógica de renderização do jogo
    void Render();
    // Encerramento do jogo (liberação de recursos de jogo)
    void Shutdown();

private:
    GLFWwindow* m_window; // Ponteiro para a janela GLFW
    float m_screenWidth;  // Largura da tela
    float m_screenHeight; // Altura da tela

    // Componentes de renderização
    Renderer2D m_renderer2D;
    // Texture* m_exampleTexture; // Removido: Não há mais textura de exemplo

    // Instâncias de Keyboard e Mouse para gerenciar o input do jogo
    Keyboard m_keyboard;
    Mouse m_mouse;

    // Funções para processar o input (chamada interna do Game)
    void processInput();

    // Funções de callback estáticas do GLFW para serem registradas pela Game
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void windowCloseCallback(GLFWwindow* window);
};

#endif // GAME_HPP