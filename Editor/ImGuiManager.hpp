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

    void DrawAssetBrowser(bool& open);
    void DrawDirectoryNode(const std::string& name, int depth);
    void DrawEditorUI(float deltaTime);

    Luna::MenuBar m_mainMenuBar;
    bool m_showSceneManager = false;
    bool m_showSpritesheetEditor = false;
    bool m_showViewport = true;

private:
    SpritesheetEditor m_spritesheetEditor;
    std::string m_assetBrowserCurrentPath; // Podemos usar isso para rastrear a "navegação" simulada
    std::string m_assetBrowserSelectedPath;
    bool m_assetBrowserRenameActive = false;
    bool m_assetBrowserCreateFolderActive = false;
    std::string m_assetBrowserDirectory;
    std::vector<std::string> GetDirectoryContents(const std::string& path); // Simulação
    std::shared_ptr<Texture> m_searchIconTexture;
};

#endif // IMGUI_MANAGER_HPP