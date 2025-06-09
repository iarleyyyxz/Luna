#include "ImGuiManager.hpp"
#include <iostream>

// Para GLFWwindow
#include <GLFW/glfw3.h> // Se você estiver usando GLFW
#include <GL/glew.h>   // Inclua GLEW aqui
#include <GL/gl.h>     // Ou outros headers OpenGL, se necessário
// ... outros includes ...
// Para Renderer2D, Keyboard, Mouse (necessário para inicializar SpritesheetEditor)
#include "Renderer2D.hpp"

#include <GL/stb_image.h>
#include <filesystem>

#include "Source/Core/Log.hpp"
#include "Console.hpp"


ImGuiManager::ImGuiManager() { // Tamanho inicial arbitrário
    std::cout << "ImGuiManager construído!" << std::endl;
}

// ... (restante do ImGuiManager::Init) ...
ImGuiManager::~ImGuiManager() {
    std::cout << "ImGuiManager destruído!" << std::endl;
}


bool ImGuiManager::Init(GLFWwindow* window, const std::string& glslVersion, Renderer2D& renderer) {
    std::cout << "Inicializando ImGuiManager..." << std::endl;

    // 1. Inicializar o contexto ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Configurar flags do ImGui
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    // 2. Aplicar o estilo Luna (Godot)
 
    m_currentTheme = EditorTheme::Dark;
    SetLunaStyle();

    // Viewports
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // 3. Inicializar os backends do ImGui para GLFW e OpenGL
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glslVersion.c_str());

    // Configura o callback do logger para enviar mensagens para o console.
    // Isso garante que todas as mensagens logadas via GLogger sejam exibidas no console.
    Luna::GLogger.SetLogCallback([this](const Luna::LogMessage& msg) {
        console.AddLog(msg);
        });

    

    // Exemplo de uso do logger após a configuração do callback
    Luna::GLogger.Info("Luna Engine inicializado.");
    Luna::GLogger.Debug("Modo de depuracao ativo.");
    Luna::GLogger.Warn("Verifique os recursos de memoria.");

    // Menu Bar
    Luna::Menu arquivoMenu;
    arquivoMenu.label = "Arquivo";
    arquivoMenu.items.push_back({ "Novo", "Ctrl+N", []() { std::cout << "Novo clicado\n"; } });
    arquivoMenu.items.push_back({ "Abrir...", "Ctrl+O", []() { std::cout << "Abrir clicado\n"; } });
    arquivoMenu.items.push_back({ "Salvar", "Ctrl+S", []() { std::cout << "Salvar clicado\n"; } });
    arquivoMenu.items.push_back({ "Salvar Como...", "", []() { std::cout << "Salvar Como clicado\n"; } });
    arquivoMenu.items.push_back({ "Sair", "Alt+F4", []() { glfwSetWindowShouldClose(glfwGetCurrentContext(), true); } });
    m_mainMenuBar.AddMenu(arquivoMenu);

    Luna::Menu editarMenu;
    editarMenu.label = "Editar";
    editarMenu.items.push_back({ "Desfazer", "Ctrl + Z", []() { std::cout << "Desfazer clicado\n"; } });
    editarMenu.items.push_back({ "Refazer", "Ctrl+Y", []() { std::cout << "Refazer clicado\n"; }, false });
    editarMenu.items.push_back({ "Recortar", "Ctrl+X", []() { std::cout << "Recortar clicado\n"; } });
    editarMenu.items.push_back({ "Copiar", "Ctrl+C", []() { std::cout << "Copiar clicado\n"; } });
    editarMenu.items.push_back({ "Colar", "Ctrl+V", []() { std::cout << "Colar clicado\n"; } });
    m_mainMenuBar.AddMenu(editarMenu);

    Luna::Menu janelasMenu;
    janelasMenu.label = "Janelas";
    janelasMenu.items.push_back({ "Gerenciador de Cenas", "", [this]() { m_showSceneManager = !m_showSceneManager; } });
    janelasMenu.items.push_back({ "Spritesheet Editor", "", [this]() { m_showSpritesheetEditor = !m_showSpritesheetEditor; } });
    janelasMenu.items.push_back({ "Viewport", "", [this]() { m_showViewport = !m_showViewport; } }); // Adicionado item da viewport
    m_mainMenuBar.AddMenu(janelasMenu);

    Luna::Menu temasMenu;
    temasMenu.label = "Temas";
    temasMenu.items.push_back({ "Light", "", [this]() {
        m_currentTheme = EditorTheme::Light;
        ApplyCurrentTheme();
    } });
    temasMenu.items.push_back({ "Dark", "", [this]() {
        m_currentTheme = EditorTheme::Dark;
        ApplyCurrentTheme();
    } });
    temasMenu.items.push_back({ "Custom", "", [this]() {
        m_currentTheme = EditorTheme::Custom;
        ApplyCurrentTheme();
    } });
    temasMenu.items.push_back({ "Purple", "", [this]() {
        m_currentTheme = EditorTheme::Purple;
        ApplyCurrentTheme();
    } });
    m_mainMenuBar.AddMenu(temasMenu);


    // NOVO: Inicializar o SpritesheetEditor

    std::cout << "ImGuiManager inicializado com sucesso." << std::endl;
    return true;
}

void ImGuiManager::BeginFrame() {
    // Iniciar o novo frame do ImGui
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // DockSpace (Comente este bloco)
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    ImGui::Begin("DockSpace Demo", nullptr, window_flags);
    ImGui::PopStyleVar(2);
    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

    // Menu Bar
    m_mainMenuBar.Draw();
}void ImGuiManager::EndFrame(GLFWwindow* window) {
    ImGui::End(); // End DockSpace

    // Renderizar as janelas (agora fora do BeginFrame/EndFrame do DockSpace)
    if (m_showSceneManager) {
        // Supondo que você tenha acesso ao SceneManager de alguma forma
        // Exemplo: extern Luna::SceneManager g_sceneManager;
        // g_sceneManager.OnImGuiRender();
    }
    

    // Renderizar o ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Lidar com viewports do ImGui (se habilitados)
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

// Adicione essas variáveis de controle como membros privados de ImGuiManager.hpp:
// bool m_showSceneManager = true;
// bool m_showSpritesheetEditor = true;

void ImGuiManager::Shutdown() {
    std::cout << "Encerrando ImGuiManager..." << std::endl;

    // NOVO: O SpritesheetEditor será destruído automaticamente como membro do ImGuiManager.
    // Não precisamos chamar Shutdown() explicitamente se ele não tiver um.

    // Desligar ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    std::cout << "ImGuiManager encerrado." << std::endl;
}

void ImGuiManager::SetLunaStyle() {
    ImGuiStyle& style = ImGui::GetStyle(); 
 }

void ImGuiManager::ApplyDarkTheme(ImGuiStyle& style) {
    ImVec4 darkBg = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    ImVec4 darkText = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
    ImVec4 darkBorder = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    ImVec4 darkFrameBg = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    ImVec4 darkFrameBgHovered = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    ImVec4 darkFrameBgActive = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    ImVec4 darkTitleBg = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    ImVec4 darkTitleBgActive = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    ImVec4 darkCheckMark = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    ImVec4 darkSliderGrab = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    ImVec4 darkSliderGrabActive = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
    ImVec4 darkButton = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    ImVec4 darkButtonHovered = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    ImVec4 darkButtonActive = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    ImVec4 darkHeader = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    ImVec4 darkHeaderHovered = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    ImVec4 darkHeaderActive = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);

    style.Colors[ImGuiCol_Text] = darkText;
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = darkBg;
    style.Colors[ImGuiCol_ChildBg] = darkBg;
    style.Colors[ImGuiCol_PopupBg] = darkBg;
    style.Colors[ImGuiCol_Border] = darkBorder;
    style.Colors[ImGuiCol_FrameBg] = darkFrameBg;
    style.Colors[ImGuiCol_FrameBgHovered] = darkFrameBgHovered;
    style.Colors[ImGuiCol_FrameBgActive] = darkFrameBgActive;
    style.Colors[ImGuiCol_TitleBg] = darkTitleBg;
    style.Colors[ImGuiCol_TitleBgActive] = darkTitleBgActive;
    style.Colors[ImGuiCol_CheckMark] = darkCheckMark;
    style.Colors[ImGuiCol_SliderGrab] = darkSliderGrab;
    style.Colors[ImGuiCol_SliderGrabActive] = darkSliderGrabActive;
    style.Colors[ImGuiCol_Button] = darkButton;
    style.Colors[ImGuiCol_ButtonHovered] = darkButtonHovered;
    style.Colors[ImGuiCol_ButtonActive] = darkButtonActive;
    style.Colors[ImGuiCol_Header] = darkHeader;
    style.Colors[ImGuiCol_HeaderHovered] = darkHeaderHovered;
    style.Colors[ImGuiCol_HeaderActive] = darkHeaderActive;
    style.Colors[ImGuiCol_MenuBarBg] = darkHeader;

    style.WindowRounding = 0.0f;
    style.FrameRounding = 0.0f;
    style.GrabRounding = 0.0f;
    style.ScrollbarRounding = 0.0f;
    style.TabRounding = 0.0f;
}

void ImGuiManager::ApplyLightTheme(ImGuiStyle& style) {
    ImVec4 white = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    ImVec4 lightGray = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
    ImVec4 gray = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
    ImVec4 black = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);

    style.Colors[ImGuiCol_Text] = black;
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = white;
    style.Colors[ImGuiCol_ChildBg] = white;
    style.Colors[ImGuiCol_PopupBg] = white;
    style.Colors[ImGuiCol_Border] = gray;
    style.Colors[ImGuiCol_FrameBg] = lightGray;
    style.Colors[ImGuiCol_FrameBgHovered] = gray;
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    style.Colors[ImGuiCol_TitleBg] = white;
    style.Colors[ImGuiCol_TitleBgActive] = lightGray;
    style.Colors[ImGuiCol_CheckMark] = black;
    style.Colors[ImGuiCol_SliderGrab] = black;
    style.Colors[ImGuiCol_SliderGrabActive] = gray;
    style.Colors[ImGuiCol_Button] = lightGray;
    style.Colors[ImGuiCol_ButtonHovered] = gray;
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    style.Colors[ImGuiCol_Header] = lightGray;
    style.Colors[ImGuiCol_HeaderHovered] = gray;
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    style.Colors[ImGuiCol_MenuBarBg] = lightGray;

    style.WindowRounding = 0.0f;
    style.FrameRounding = 0.0f;
    style.GrabRounding = 0.0f;
    style.ScrollbarRounding = 0.0f;
    style.TabRounding = 0.0f;
}


void ImGuiManager::ApplyCustomTheme(ImGuiStyle& style) {
    ImVec4 bg = ImVec4(0.12f, 0.14f, 0.18f, 1.00f);  // Near black with a blue tint
    ImVec4 panel = ImVec4(0.17f, 0.20f, 0.26f, 1.00f);  // Panels (frames, popups)
    ImVec4 hover = ImVec4(0.25f, 0.29f, 0.38f, 1.00f);  // Hover state
    ImVec4 active = ImVec4(0.35f, 0.40f, 0.52f, 1.00f);  // Active state
    ImVec4 accent = ImVec4(0.20f, 0.55f, 0.85f, 1.00f);  // Main color (vibrant blue)
    ImVec4 text = ImVec4(0.92f, 0.95f, 0.98f, 1.00f);  // Light text
    ImVec4 textMuted = ImVec4(0.55f, 0.60f, 0.70f, 1.00f);  // Muted text
    ImVec4 border = ImVec4(0.22f, 0.25f, 0.30f, 1.00f);  // Soft borders

    // Base colors
    style.Colors[ImGuiCol_Text] = text;
    style.Colors[ImGuiCol_TextDisabled] = textMuted;
    style.Colors[ImGuiCol_WindowBg] = bg;
    style.Colors[ImGuiCol_ChildBg] = bg;
    style.Colors[ImGuiCol_PopupBg] = panel;
    style.Colors[ImGuiCol_Border] = border;
    style.Colors[ImGuiCol_BorderShadow] = bg;

    // Frame & widget colors
    style.Colors[ImGuiCol_FrameBg] = panel;
    style.Colors[ImGuiCol_FrameBgHovered] = hover;
    style.Colors[ImGuiCol_FrameBgActive] = active;

    // Title bar
    style.Colors[ImGuiCol_TitleBg] = panel;
    style.Colors[ImGuiCol_TitleBgActive] = hover;
    style.Colors[ImGuiCol_TitleBgCollapsed] = panel;

    // Buttons
    style.Colors[ImGuiCol_Button] = panel;
    style.Colors[ImGuiCol_ButtonHovered] = hover;
    style.Colors[ImGuiCol_ButtonActive] = active;

    // Header (e.g., collapsing headers)
    style.Colors[ImGuiCol_Header] = panel;
    style.Colors[ImGuiCol_HeaderHovered] = hover;
    style.Colors[ImGuiCol_HeaderActive] = active;

    // Sliders & check marks
    style.Colors[ImGuiCol_SliderGrab] = accent;
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(accent.x, accent.y, accent.z, 1.0f);
    style.Colors[ImGuiCol_CheckMark] = accent;

    // Tabs
    style.Colors[ImGuiCol_Tab] = panel;
    style.Colors[ImGuiCol_TabHovered] = hover;
    style.Colors[ImGuiCol_TabActive] = active;
    style.Colors[ImGuiCol_TabUnfocused] = panel;
    style.Colors[ImGuiCol_TabUnfocusedActive] = hover;

    // Menubar
    style.Colors[ImGuiCol_MenuBarBg] = panel;

    // Scrollbar
    style.Colors[ImGuiCol_ScrollbarBg] = bg;
    style.Colors[ImGuiCol_ScrollbarGrab] = hover;
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = active;
    style.Colors[ImGuiCol_ScrollbarGrabActive] = accent;

    // Resize grip
    style.Colors[ImGuiCol_ResizeGrip] = hover;
    style.Colors[ImGuiCol_ResizeGripHovered] = active;
    style.Colors[ImGuiCol_ResizeGripActive] = accent;

    // Separator
    style.Colors[ImGuiCol_Separator] = border;
    style.Colors[ImGuiCol_SeparatorHovered] = hover;
    style.Colors[ImGuiCol_SeparatorActive] = active;

    // Style geometry
    style.WindowRounding = 6.0f;
    style.FrameRounding = 4.0f;
    style.GrabRounding = 4.0f;
    style.ScrollbarRounding = 4.0f;
    style.TabRounding = 4.0f;

    style.WindowPadding = ImVec2(8.0f, 8.0f);
    style.FramePadding = ImVec2(6.0f, 4.0f);
    style.ItemSpacing = ImVec2(6.0f, 6.0f);
}

void ImGuiManager::ApplyPurpleTheme(ImGuiStyle& style) {
    ImVec4 bg = ImVec4(0.12f, 0.10f, 0.16f, 1.00f);  // Dark purple-ish background
    ImVec4 panel = ImVec4(0.18f, 0.14f, 0.26f, 1.00f);  // Panel/frame color
    ImVec4 hover = ImVec4(0.35f, 0.28f, 0.55f, 1.00f);  // Hover state
    ImVec4 active = ImVec4(0.48f, 0.36f, 0.75f, 1.00f);  // Active state
    ImVec4 accent = ImVec4(0.65f, 0.45f, 0.85f, 1.00f);  // Primary accent (lavender purple)
    ImVec4 text = ImVec4(0.95f, 0.95f, 0.97f, 1.00f);  // Light text
    ImVec4 textMuted = ImVec4(0.60f, 0.60f, 0.70f, 1.00f);  // Muted text
    ImVec4 border = ImVec4(0.30f, 0.25f, 0.35f, 1.00f);  // Subtle border

    // Base colors
    style.Colors[ImGuiCol_Text] = text;
    style.Colors[ImGuiCol_TextDisabled] = textMuted;
    style.Colors[ImGuiCol_WindowBg] = bg;
    style.Colors[ImGuiCol_ChildBg] = bg;
    style.Colors[ImGuiCol_PopupBg] = panel;
    style.Colors[ImGuiCol_Border] = border;
    style.Colors[ImGuiCol_BorderShadow] = bg;

    // Frame & widget colors
    style.Colors[ImGuiCol_FrameBg] = panel;
    style.Colors[ImGuiCol_FrameBgHovered] = hover;
    style.Colors[ImGuiCol_FrameBgActive] = active;

    // Title bar
    style.Colors[ImGuiCol_TitleBg] = panel;
    style.Colors[ImGuiCol_TitleBgActive] = hover;
    style.Colors[ImGuiCol_TitleBgCollapsed] = panel;

    // Buttons
    style.Colors[ImGuiCol_Button] = panel;
    style.Colors[ImGuiCol_ButtonHovered] = hover;
    style.Colors[ImGuiCol_ButtonActive] = active;

    // Header (e.g., collapsing headers)
    style.Colors[ImGuiCol_Header] = panel;
    style.Colors[ImGuiCol_HeaderHovered] = hover;
    style.Colors[ImGuiCol_HeaderActive] = active;

    // Sliders & check marks
    style.Colors[ImGuiCol_SliderGrab] = accent;
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(accent.x, accent.y, accent.z, 1.00f);
    style.Colors[ImGuiCol_CheckMark] = accent;

    // Tabs
    style.Colors[ImGuiCol_Tab] = panel;
    style.Colors[ImGuiCol_TabHovered] = hover;
    style.Colors[ImGuiCol_TabActive] = active;
    style.Colors[ImGuiCol_TabUnfocused] = panel;
    style.Colors[ImGuiCol_TabUnfocusedActive] = hover;

    // Menubar
    style.Colors[ImGuiCol_MenuBarBg] = panel;

    // Scrollbar
    style.Colors[ImGuiCol_ScrollbarBg] = bg;
    style.Colors[ImGuiCol_ScrollbarGrab] = hover;
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = active;
    style.Colors[ImGuiCol_ScrollbarGrabActive] = accent;

    // Resize grip
    style.Colors[ImGuiCol_ResizeGrip] = hover;
    style.Colors[ImGuiCol_ResizeGripHovered] = active;
    style.Colors[ImGuiCol_ResizeGripActive] = accent;

    // Separator
    style.Colors[ImGuiCol_Separator] = border;
    style.Colors[ImGuiCol_SeparatorHovered] = hover;
    style.Colors[ImGuiCol_SeparatorActive] = active;

    // Style geometry
    style.WindowRounding = 6.0f;
    style.FrameRounding = 5.0f;
    style.GrabRounding = 4.0f;
    style.ScrollbarRounding = 4.0f;
    style.TabRounding = 5.0f;

    style.WindowPadding = ImVec2(8.0f, 8.0f);
    style.FramePadding = ImVec2(6.0f, 4.0f);
    style.ItemSpacing = ImVec2(6.0f, 6.0f);
}

void ImGuiManager::ApplyCurrentTheme() {
    switch (m_currentTheme) {
    case EditorTheme::Light: ApplyLightTheme(ImGui::GetStyle()); break;
    case EditorTheme::Dark: ApplyDarkTheme(ImGui::GetStyle()); break;
    case EditorTheme::Custom: ApplyCustomTheme(ImGui::GetStyle()); break;
    case EditorTheme::Purple: ApplyPurpleTheme(ImGui::GetStyle()); break;
    default: ApplyDarkTheme(ImGui::GetStyle());
    }
}

void ImGuiManager::ShowConsole() {
    console.OnGui();
}


bool ImGuiManager::LoadFont(const std::string& fontPath, float fontSize) {
    ImGuiIO& io = ImGui::GetIO();
    ImFont* font = io.Fonts->AddFontFromFileTTF(fontPath.c_str(), fontSize);
    if (font == nullptr) {
        std::cerr << "ERRO::IMGUI_MANAGER: Falha ao carregar a fonte: " << fontPath << std::endl;
        return false;
    }
    std::cout << "Fonte '" << fontPath << "' carregada com sucesso, tamanho: " << fontSize << std::endl;
    return true;
}

bool ImGuiManager::LoadFontAwesome(const std::string& fontAwesomePath, float fontSize) {
    ImGuiIO& io = ImGui::GetIO();

    // Define a faixa Unicode dos ícones FontAwesome (fa-solid)
    static const ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 };

    ImFontConfig icons_config;
    icons_config.MergeMode = true;       // mescla essa fonte com a fonte atual
    icons_config.PixelSnapH = true;

    ImFont* fontAwesome = io.Fonts->AddFontFromFileTTF(fontAwesomePath.c_str(), fontSize, &icons_config, icons_ranges);
    if (fontAwesome == nullptr) {
        std::cerr << "ERRO::IMGUI_MANAGER: Falha ao carregar a fonte FontAwesome: " << fontAwesomePath << std::endl;
        return false;
    }
    std::cout << "Fonte FontAwesome '" << fontAwesomePath << "' carregada e mesclada com sucesso, tamanho: " << fontSize << std::endl;
    return true;
}

void ImGuiManager::DrawEditorUI(float deltaTime) {
    // DockSpace está ativo no BeginFrame/EndFrame
   /* if (m_showViewport) {
        ImGui::Begin("Viewport");
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        ImGui::Image(m_renderTarget.getTextureID(), viewportPanelSize, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();
    }*/

    ShowConsole();


  //  this->assetBrowser.render();
}