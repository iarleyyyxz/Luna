#ifndef APPLICATION_H
#define APPLICATION_H

#include <functional>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Source/Events/EventManager.hpp" // Certifique-se de que este caminho est� correto
// #include "InputManager.hpp" // Removido: InputManager agora � gerenciado pela Game
#include "Game.hpp" // Para a inst�ncia da classe Game

class Application
{
public:
    Application();
    ~Application();

    bool Init();
    void Run();
    void Shutdown();

    bool IsRunning() const { return isRunning; }

    // Event delegates
    std::function<void()> OnStep;
    std::function<void()> OnExit;
    std::function<void(const std::string&)> OnNewScene;

    // Acesso ao EventManager
    EventManager& GetEventManager() { return eventManager; }

    // Removido: GetKeyboard() e GetMouse() n�o s�o mais necess�rios aqui
    // Keyboard& GetKeyboard() { return keyboard; }
    // Mouse& GetMouse() { return mouse; }

private:
    bool isRunning;
    GLFWwindow* window; // Ponteiro para a janela GLFW
    EventManager eventManager; // Inst�ncia do EventManager
    // Removido: Keyboard keyboard;
    // Removido: Mouse mouse;
    Game game; // Inst�ncia da classe Game

    float screenWidth;
    float screenHeight;

    // Removido: Fun��es de callback est�ticas do GLFW (agora na Game)
    static void glfwErrorCallback(int error, const char* description);
    // static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    // static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
    // static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void windowCloseCallback(GLFWwindow* window); // Manter se Application ainda precisar de um callback de fechar janela
};

#endif // APPLICATION_H
