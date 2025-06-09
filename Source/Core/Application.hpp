// Application.hpp
#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <functional>
#include <string>
#include <memory> // Para std::unique_ptr

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Source/Events/EventManager.hpp"
#include "Renderer2D.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "Editor/ImGuiManager.hpp"
#include "Editor/ViewportGui.hpp"
#include "Texture.hpp" // Incluir Texture
#include "Sprite.hpp"  // Incluir Sprite
#include <glm.hpp>                 // Incluir glm para vetores
#include "Camera2D.hpp"
#include "Source/Scene/SceneManager.hpp"
#include "Editor/Gizmo2D.hpp"
// Estrutura de dados para a luz
class Application
{
public:
    Application();
    ~Application();

    bool Init();
    void Run();
    void Shutdown();

    bool IsRunning() const { return isRunning; }

    std::function<void()> OnStep;
    std::function<void()> OnExit;
    std::function<void(const std::string&)> OnNewScene;

    EventManager& GetEventManager() { return eventManager; }
    Keyboard& GetKeyboard() { return m_keyboard; }
    Luna::Input::Mouse& GetMouse() { return m_mouse; }
    unsigned int GetFramebufferTexture() const { return framebufferTexture; }

    void ProcessInput(GLFWwindow* wwindow);

    Luna::Editor::Gizmo2D m_gizmo;
    std::shared_ptr<SceneObject> m_selectedObjectForGizmo; // O objeto que o gizmo manipulará

    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    glm::vec4 GetTileColor(int tileId);

    ImFont* fontIcons; // fonte com ícones


private:
    bool isRunning;
    GLFWwindow* window;

    float screenWidth;
    float screenHeight;

    Renderer2D m_renderer2D;
    Keyboard m_keyboard;
    Luna::Input::Mouse m_mouse;
    ImGuiManager m_imGuiManager;
    ViewportGui m_viewportGui;

    float deltaTime;

    EventManager eventManager;

    unsigned int framebufferTexture;
    unsigned int framebufferObject;
    bool InitFramebuffer();

    // Sprite to render
    // ... (outros membros)
    std::shared_ptr<Texture> m_spriteTexture;
    Sprite m_testSprite; // Apenas declaração aqui
    // ...
    // ... outros membros ...
    std::vector<std::shared_ptr<SceneObject>> m_sceneObjects;
    Camera2D m_camera;
    SceneManager& sceneManager = SceneManager::GetInstance();

    bool m_isGizmoDragging = false;
    glm::vec2 m_dragOffset; // Offset entre a posição do mouse e o centro do gizmo ao iniciar o drag


    static const float GRID_SPACING; // Apenas declaração
    static const glm::vec4 GRID_COLOR; // Apenas declaração
    void RenderGrid();

    static void glfwErrorCallback(int error, const char* description);
    static void windowCloseCallback(GLFWwindow* window);

    // ... outros membros privados ...
    bool m_isDragging = false;
    glm::vec2 m_dragStartPosition;
   // void processMousePan(const glm::vec2& currentMousePosition);
};

#endif // APPLICATION_HPP