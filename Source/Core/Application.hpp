#ifndef APPLICATION_H
#define APPLICATION_H

#include <functional>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Source/Events/EventManager.hpp" // Certifique-se de que este caminho está correto
#include "Renderer2D.hpp"   // Para a instância do Renderer2D
#include "InputManager.hpp" // Para Keyboard e Mouse
#include "Source/Scene/SceneManager.hpp" // Para a instância do SceneManager

// Incluir o cabeçalho do ImGuiManager
#include "Editor/ImGuiManager.hpp"
// REMOVIDO: Inclusão direta do SpritesheetEditor, pois agora é gerenciado pelo ImGuiManager
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
    EventManager eventManager; // Instância do EventManager

    // Recursos da engine que serão gerenciados pela Application e passados para as cenas
    Renderer2D renderer2D;
    Keyboard keyboard;
    Mouse mouse;

    // Instância do SceneManager
    SceneManager sceneManager;

    // REMOVIDO: Instância do SpritesheetEditor
    // SpritesheetEditor spritesheetEditor;

    // Instância do ImGuiManager (agora gerencia o SpritesheetEditor)
    ImGuiManager imGuiManager;

    float screenWidth;
    float screenHeight;

    // Funções de callback estáticas do GLFW
    static void glfwErrorCallback(int error, const char* description);
    static void windowCloseCallback(GLFWwindow* window);
};

#endif // APPLICATION_H