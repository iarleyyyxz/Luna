#ifndef GAME_HPP
#define GAME_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <string>

#include "Renderer2D.hpp"
#include "InputManager.hpp"
#include "Camera2D.hpp"
#include "Sprite.hpp"
#include "Spritesheet.hpp"
#include "Anim/Animation.hpp"    // NOVO: Para a classe Animation

class Game {
public:
    Game();
    ~Game();

    bool Init(GLFWwindow* window, float screenWidth, float screenHeight);
    void Run();
    void Update(float deltaTime);
    void Render();
    void Shutdown();

private:
    GLFWwindow* m_window;
    float m_screenWidth;
    float m_screenHeight;

    Renderer2D m_renderer2D;
    Camera2D m_camera;

   // Spritesheet* m_mainSpritesheet; // A spritesheet principal
    // Sprite* m_playerSprite; // Removido: Agora o sprite é gerenciado pela animação

    // NOVO: Animação do jogador
 //   Animation* m_playerAnimation;
 //   std::vector<Sprite*> m_playerWalkFrames; // Vetor para armazenar os frames da animação de caminhada

    // Instâncias de Keyboard e Mouse
    Keyboard m_keyboard;
    Mouse m_mouse;

    void processInput();

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void windowCloseCallback(GLFWwindow* window);

    // Tempo para calcular o deltaTime
    double m_lastFrameTime;
};

#endif // GAME_HPP