#ifndef IMGUI_MANAGER_HPP
#define IMGUI_MANAGER_HPP

#define IMGUI_ENABLE_DOCKING
#include "imgui.h"         // Core ImGui
#include "imgui_internal.h" // Opcional, para funcionalidades avançadas
#include "imgui_impl_glfw.h" // ImGui para GLFW
#include "imgui_impl_opengl3.h" // ImGui para OpenGL

#include <string> // Para caminhos de fonte
#include "RenderTarget.hpp" // <--- Inclua este header

// NOVO: Incluir o cabeçalho do SpritesheetEditor
#include "Menubar.hpp"
#include "SpritesheetEditor.hpp"

// Forward declarations para GLFWwindow e os gerenciadores de recursos
struct GLFWwindow;
class Renderer2D; // Declarar antes de usar em Init
class Keyboard;   // Declarar antes de usar em Init
class Mouse;      // Declarar antes de usar em Init

class ImGuiManager {
public:
    ImGuiManager();
    ~ImGuiManager();

    // Inicializa o contexto ImGui e os backends para GLFW e OpenGL.
    // window: Ponteiro para a janela GLFW.
    // glslVersion: String da versão GLSL (ex: "#version 330 core").
    // NOVO: Adicionado renderer, keyboard e mouse para inicializar SpritesheetEditor
    bool Init(GLFWwindow* window, const std::string& glslVersion, Renderer2D& renderer, Keyboard& keyboard, Mouse& mouse);

    // Inicia um novo frame do ImGui. Deve ser chamado no início do loop de renderização.
    void BeginFrame();

    // Finaliza o frame do ImGui e o renderiza. Deve ser chamado no final do loop de renderização.
    void EndFrame(GLFWwindow* window);

    // Encerra o contexto ImGui e libera os recursos.
    void Shutdown();

    // Aplica um estilo visual semelhante ao tema escuro da Godot.
    void SetLunaStyle();

    // Carrega uma fonte personalizada.
    // fontPath: Caminho para o arquivo da fonte (ex: "Resources/Fonts/Roboto-Medium.ttf").
    // fontSize: Tamanho da fonte.
    // Retorna true se a fonte foi carregada com sucesso.
    bool LoadFont(const std::string& fontPath, float fontSize);

    // NOVO: Desenha a interface do utilizador do SpritesheetEditor
    void DrawEditorUI(float deltaTime);

    // handlers
    Luna::MenuBar m_mainMenuBar;
    bool m_showSceneManager = false; // Inicializado como false
    bool m_showSpritesheetEditor = false; // Inicializado como false
    bool m_showViewport = true; // Inicializado como true

private:
    // NOVO: Instância do RenderTarget para a viewport
   // Luna::RenderTarget m_renderTarget;
    // NOVO: Instância do SpritesheetEditor gerenciada pelo ImGuiManager
    SpritesheetEditor m_spritesheetEditor;
};

#endif // IMGUI_MANAGER_HPP