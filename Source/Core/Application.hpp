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
#include "Source/Renderer/Texture.hpp" // Incluir Texture
#include "Source/Renderer/Sprite.hpp"  // Incluir Sprite
#include <glm.hpp>                 // Incluir glm para vetores
#include "Camera2D.hpp"

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
    Mouse& GetMouse() { return m_mouse; }
    unsigned int GetFramebufferTexture() const { return framebufferTexture; }

private:
    bool isRunning;
    GLFWwindow* window;

    float screenWidth;
    float screenHeight;

    Renderer2D m_renderer2D;
    Keyboard m_keyboard;
    Mouse m_mouse;
    ImGuiManager m_imGuiManager;
    ViewportGui m_viewportGui;

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
    // ...

    static const float GRID_SPACING; // Apenas declaração
    static const glm::vec4 GRID_COLOR; // Apenas declaração
    void RenderGrid();

    static void glfwErrorCallback(int error, const char* description);
    static void windowCloseCallback(GLFWwindow* window);
};

#endif // APPLICATION_HPP