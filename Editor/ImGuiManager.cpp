#include "ImGuiManager.hpp"
#include <iostream>

// Para GLFWwindow
#include <GLFW/glfw3.h>
// Para Renderer2D, Keyboard, Mouse (necessário para inicializar SpritesheetEditor)
#include "Renderer2D.hpp"
#include "InputManager.hpp"

ImGuiManager::ImGuiManager() : m_spritesheetEditor() { // NOVO: Inicializa m_spritesheetEditor
    std::cout << "ImGuiManager construído!" << std::endl;
}

ImGuiManager::~ImGuiManager() {
    std::cout << "ImGuiManager destruído!" << std::endl;
}

bool ImGuiManager::Init(GLFWwindow* window, const std::string& glslVersion, Renderer2D& renderer, Keyboard& keyboard, Mouse& mouse) { // NOVO: Parâmetros adicionais
    std::cout << "Inicializando ImGuiManager..." << std::endl;

    // 1. Inicializar o contexto ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Configurar flags do ImGui
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Habilitar navegação por teclado
//    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Habilitar docking (para layout de editor)
//    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;     // Habilitar viewports (para janelas fora da janela principal)
    // 2. Aplicar o estilo Godot
    SetLunaStyle();

    // Quando viewports estão habilitados, o ImGui criará janelas GLFW/contextos OpenGL adicionais
    // para cada viewport, e precisará de callbacks GLFW.
    if (io.ConfigFlags)
    {
        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // 3. Inicializar os backends do ImGui para GLFW e OpenGL
    ImGui_ImplGlfw_InitForOpenGL(window, true); // O 'true' instala os callbacks do ImGui para o GLFW
    ImGui_ImplOpenGL3_Init(glslVersion.c_str()); // Versão do GLSL

    // 4. Carregar uma fonte padrão (opcional, pode ser substituída por LoadFont)
    // io.Fonts->AddFontDefault(); // Fonte padrão do ImGui

    // NOVO: Inicializar o SpritesheetEditor que agora é membro do ImGuiManager
    m_spritesheetEditor.Init(renderer, keyboard, mouse);

    std::cout << "ImGuiManager inicializado com sucesso." << std::endl;
    return true;
}

void ImGuiManager::BeginFrame() {
    // Iniciar o novo frame do ImGui
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiManager::EndFrame(GLFWwindow* window) {
    // Renderizar o ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Lidar com viewports do ImGui (se habilitados)
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
   //     ImGui::UpdatePlatformWindows();
   //     ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

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

    // Cores base do Godot (aproximadas para o tema escuro)
    ImVec4 godotBg = ImVec4(0.18f, 0.20f, 0.23f, 1.00f); // Janelas, fundos
    ImVec4 godotHeader = ImVec4(0.25f, 0.28f, 0.32f, 1.00f); // Cabeçalhos de árvores, menus
    ImVec4 godotActive = ImVec4(0.35f, 0.45f, 0.80f, 1.00f); // Elementos ativos, botões pressionados
    ImVec4 godotHover = ImVec4(0.40f, 0.50f, 0.85f, 1.00f); // Elementos ao passar o mouse
    ImVec4 godotText = ImVec4(0.90f, 0.90f, 0.90f, 1.00f); // Texto
    ImVec4 godotFrameBg = ImVec4(0.15f, 0.17f, 0.20f, 1.00f); // Fundo de frames (inputs, sliders)
    ImVec4 godotBorder = ImVec4(0.10f, 0.10f, 0.10f, 1.00f); // Bordas
    ImVec4 godotTitleBg = ImVec4(0.15f, 0.17f, 0.20f, 1.00f); // Título da janela

    // Aplicar cores
    style.Colors[ImGuiCol_Text] = godotText;
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = godotBg;
    style.Colors[ImGuiCol_ChildBg] = godotBg;
    style.Colors[ImGuiCol_PopupBg] = godotBg;
    style.Colors[ImGuiCol_Border] = godotBorder;
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg] = godotFrameBg;
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.20f, 0.22f, 0.25f, 1.00f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.25f, 0.28f, 0.32f, 1.00f);
    style.Colors[ImGuiCol_TitleBg] = godotTitleBg;
    style.Colors[ImGuiCol_TitleBgActive] = godotTitleBg;
    style.Colors[ImGuiCol_TitleBgCollapsed] = godotTitleBg;
    style.Colors[ImGuiCol_MenuBarBg] = godotBg;
    style.Colors[ImGuiCol_ScrollbarBg] = godotFrameBg;
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    style.Colors[ImGuiCol_CheckMark] = godotActive;
    style.Colors[ImGuiCol_SliderGrab] = godotActive;
    style.Colors[ImGuiCol_SliderGrabActive] = godotHover;
    style.Colors[ImGuiCol_Button] = godotHeader;
    style.Colors[ImGuiCol_ButtonHovered] = godotHover;
    style.Colors[ImGuiCol_ButtonActive] = godotActive;
    style.Colors[ImGuiCol_Header] = godotHeader; // Para CollapsingHeader
    style.Colors[ImGuiCol_HeaderHovered] = godotHover;
    style.Colors[ImGuiCol_HeaderActive] = godotActive;
    style.Colors[ImGuiCol_Separator] = godotBorder;
    style.Colors[ImGuiCol_SeparatorHovered] = godotHover;
    style.Colors[ImGuiCol_SeparatorActive] = godotActive;
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.40f, 0.40f, 0.29f);
    style.Colors[ImGuiCol_Tab] = godotHeader;
    style.Colors[ImGuiCol_TabHovered] = godotHover;
    style.Colors[ImGuiCol_TabActive] = godotActive;
    style.Colors[ImGuiCol_TabUnfocused] = godotHeader;
    style.Colors[ImGuiCol_TabUnfocusedActive] = godotActive;
 //   style.Colors[ImGuiCol_DockingPreview] = godotActive;
 //   style.Colors[ImGuiCol_DockingEmptyBg] = godotBg;
    style.Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg] = godotActive;
    style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    style.Colors[ImGuiCol_NavHighlight] = godotActive;
    style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

    // Ajustes de arredondamento e espaçamento para um visual mais "Godot"
    style.WindowRounding = 4.0f;
    style.FrameRounding = 3.0f;
    style.GrabRounding = 3.0f;
    style.ScrollbarRounding = 3.0f;
    style.TabRounding = 3.0f;

    style.WindowPadding = ImVec2(8.0f, 8.0f);
    style.FramePadding = ImVec2(4.0f, 3.0f);
    style.ItemSpacing = ImVec2(8.0f, 4.0f);
    style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
    style.IndentSpacing = 20.0f;
    style.ScrollbarSize = 12.0f;
    style.GrabMinSize = 10.0f;
    style.ChildRounding = 4.0f;
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

void ImGuiManager::DrawEditorUI(float deltaTime) { // NOVO: Implementação do método de desenho
    m_spritesheetEditor.DrawUI(deltaTime);
}
