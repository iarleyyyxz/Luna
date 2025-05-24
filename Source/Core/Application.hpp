#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <functional>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Source/Events/EventManager.hpp" // Certifique-se de que este caminho está correto
#include "Renderer2D.hpp"   // Para a instância do Renderer2D
#include "InputManager.hpp" // Para Keyboard e Mouse
//#include "Source/Scene/SceneManager.hpp" // Para o SceneManager
#include "Editor/ImGuiManager.hpp" // Para o ImGuiManager

class Application
{
public:
    Application();
    ~Application();

    // Inicializa a aplicação (GLFW, GLEW, Renderer2D, SceneManager, ImGuiManager)
    bool Init();
    // Executa o loop principal da aplicação
    void Run();
    // Encerra a aplicação (libera recursos)
    void Shutdown();

    bool IsRunning() const { return isRunning; }

    // Event delegates (mantidos da sua estrutura original)
    std::function<void()> OnStep;
    std::function<void()> OnExit;
    std::function<void(const std::string&)> OnNewScene;

    // Acesso ao EventManager
    EventManager& GetEventManager() { return eventManager; }

    // Acesso às instâncias de Keyboard e Mouse (ainda podem ser úteis para acesso global)
    Keyboard& GetKeyboard() { return m_keyboard; }
    Mouse& GetMouse() { return m_mouse; }

private:
    bool isRunning;          // Flag para controlar o loop da aplicação
    GLFWwindow* window;      // Ponteiro para a janela GLFW

    float screenWidth;       // Largura da janela
    float screenHeight;      // Altura da janela

    Renderer2D m_renderer2D;   // Instância do Renderer2D
    Keyboard m_keyboard;       // Instância do Keyboard
    Mouse m_mouse;             // Instância do Mouse
  //  Luna::SceneManager m_sceneManager; // Instância do SceneManager
    ImGuiManager m_imGuiManager; // Instância do ImGuiManager

    EventManager eventManager; // Instância do EventManager

    // Funções de callback estáticas do GLFW (agora membros da Application)
    static void glfwErrorCallback(int error, const char* description);
    static void windowCloseCallback(GLFWwindow* window);
    // Os callbacks de input (keyCallback, cursorPositionCallback, mouseButtonCallback)
    // são agora gerenciados internamente pelo ImGuiManager e pelo Game/SceneManager.
    // Se você precisar de acesso direto a eles na Application, precisará reintroduzi-los.
};

#endif // APPLICATION_HPP