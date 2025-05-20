#ifndef GAME_HPP
#define GAME_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h> // Necess�rio para GLFWwindow* e callbacks
#include <iostream>
#include <vector>
#include <string>

#include "Renderer2D.hpp"
#include "InputManager.hpp" // Para Keyboard e Mouse
// #include "Texture.hpp"      // Removido: N�o � mais necess�rio se a Game n�o gerencia texturas

class Game {
public:
    // Construtor padr�o
    Game();
    // Destrutor
    ~Game();

    // Inicializa��o do jogo
    // Agora recebe o ponteiro da janela GLFW e suas dimens�es
    bool Init(GLFWwindow* window, float screenWidth, float screenHeight);
    // M�todo principal de execu��o do jogo para um frame (chamado pela Application)
    void Run();
    // L�gica de atualiza��o do jogo
    void Update(float deltaTime);
    // L�gica de renderiza��o do jogo
    void Render();
    // Encerramento do jogo (libera��o de recursos de jogo)
    void Shutdown();

private:
    GLFWwindow* m_window; // Ponteiro para a janela GLFW
    float m_screenWidth;  // Largura da tela
    float m_screenHeight; // Altura da tela

    // Componentes de renderiza��o
    Renderer2D m_renderer2D;
    // Texture* m_exampleTexture; // Removido: N�o h� mais textura de exemplo

    // Inst�ncias de Keyboard e Mouse para gerenciar o input do jogo
    Keyboard m_keyboard;
    Mouse m_mouse;

    // Fun��es para processar o input (chamada interna do Game)
    void processInput();

    // Fun��es de callback est�ticas do GLFW para serem registradas pela Game
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void windowCloseCallback(GLFWwindow* window);
};

#endif // GAME_HPP