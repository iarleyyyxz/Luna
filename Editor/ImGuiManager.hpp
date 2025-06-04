#ifndef IMGUI_MANAGER_HPP
#define IMGUI_MANAGER_HPP

#define IMGUI_ENABLE_DOCKING
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <string>
#include <vector> // Para simular a lista de diretórios

#include "Menubar.hpp"
#include "SpritesheetEditor.hpp"
#include "Console.hpp"

enum class EditorTheme {
    Dark,
    Light,
    Custom,
    Purple
};
class ImGuiManager {
public:
    ImGuiManager();
    ~ImGuiManager();

    bool Init(GLFWwindow* window, const std::string& glslVersion, Renderer2D& renderer, Keyboard& keyboard, Mouse& mouse);
    void BeginFrame();
    void EndFrame(GLFWwindow* window);
    void Shutdown();
    void SetLunaStyle();
    bool LoadFont(const std::string& fontPath, float fontSize);

    void DrawEditorUI(float deltaTime);

    bool open = true;

    Luna::MenuBar m_mainMenuBar;

    bool m_showSceneManager = false;
    bool m_showSpritesheetEditor = false;
    bool m_showViewport = true;

    void ApplyCurrentTheme();

    EditorTheme m_currentTheme = EditorTheme::Dark;

    void ApplyDarkTheme(ImGuiStyle& style);
    void ApplyLightTheme(ImGuiStyle& style);
    void ApplyCustomTheme(ImGuiStyle& style);
    void ApplyPurpleTheme(ImGuiStyle& style);

    static std::string selectedFolder;
    static std::string searchQuery;
    static std::map<std::string, ImTextureID> folderIcons;

    void ShowConsole();

private:
    SpritesheetEditor m_spritesheetEditor;

    Console console;
    
    std::shared_ptr<Texture> m_searchIconTexture;
    std::map<std::string, std::shared_ptr<Texture>> m_loadedTextures;
};

#endif // IMGUI_MANAGER_HPP