#ifndef APPLICATION_H
#define APPLICATION_H

#include <functional>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Source/Events/EventManager.hpp" // Certifique-se de que este caminho est� correto
#include "Renderer2D.hpp"   // Para a inst�ncia do Renderer2D
#include "InputManager.hpp" // Para Keyboard e Mouse
#include "Source/Scene/SceneManager.hpp" // Para a inst�ncia do SceneManager

// Incluir o cabe�alho do ImGuiManager
#include "Editor/ImGuiManager.hpp"
// REMOVIDO: Inclus�o direta do SpritesheetEditor, pois agora � gerenciado pelo ImGuiManager
// #include "SpritesheetEditor.hpp"

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

    // Acesso aos gerenciadores de input (para passar para as cenas)
    Keyboard& GetKeyboard() { return keyboard; }
    Mouse& GetMouse() { return mouse; }
    Renderer2D& GetRenderer() { return renderer2D; } // Acesso ao Renderer2D

private:
    bool isRunning;
    GLFWwindow* window; // Ponteiro para a janela GLFW
    EventManager eventManager; // Inst�ncia do EventManager

    // Recursos da engine que ser�o gerenciados pela Application e passados para as cenas
    Renderer2D renderer2D;
    Keyboard keyboard;
    Mouse mouse;

    Luna::SceneManager sceneManager;

    // REMOVIDO: Inst�ncia do SpritesheetEditor
    // SpritesheetEditor spritesheetEditor;

    // Inst�ncia do ImGuiManager (agora gerencia o SpritesheetEditor)
    ImGuiManager imGuiManager;

    float screenWidth;
    float screenHeight;

    // Fun��es de callback est�ticas do GLFW
    static void glfwErrorCallback(int error, const char* description);
    static void windowCloseCallback(GLFWwindow* window);
};

#endif // APPLICATION_H