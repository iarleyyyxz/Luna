#include "ImGuiManager.hpp"
#include <iostream>

// Para GLFWwindow
#include <GLFW/glfw3.h> // Se voc� estiver usando GLFW
#include <GL/glew.h>   // Inclua GLEW aqui
#include <GL/gl.h>     // Ou outros headers OpenGL, se necess�rio
// ... outros includes ...
// Para Renderer2D, Keyboard, Mouse (necess�rio para inicializar SpritesheetEditor)
#include "Renderer2D.hpp"
#include "InputManager.hpp"

#include "RenderTarget.hpp" // Certifique-se de incluir aqui tamb�m

ImGuiManager::ImGuiManager() : m_spritesheetEditor() { // Tamanho inicial arbitr�rio
    std::cout << "ImGuiManager constru�do!" << std::endl;
}

// ... (restante do ImGuiManager::Init) ...
ImGuiManager::~ImGuiManager() {
    std::cout << "ImGuiManager destru�do!" << std::endl;
}


bool ImGuiManager::Init(GLFWwindow* window, const std::string& glslVersion, Renderer2D& renderer, Keyboard& keyboard, Mouse& mouse) {
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
    editarMenu.items.push_back({ "Desfazer", "Ctrl+Z", []() { std::cout << "Desfazer clicado\n"; } });
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

    // NOVO: Inicializar o SpritesheetEditor
    m_spritesheetEditor.Init(renderer, keyboard, mouse);

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
        // Supondo que voc� tenha acesso ao SceneManager de alguma forma
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

// Adicione essas vari�veis de controle como membros privados de ImGuiManager.hpp:
// bool m_showSceneManager = true;
// bool m_showSpritesheetEditor = true;

void ImGuiManager::Shutdown() {
    std::cout << "Encerrando ImGuiManager..." << std::endl;

    // NOVO: O SpritesheetEditor ser� destru�do automaticamente como membro do ImGuiManager.
    // N�o precisamos chamar Shutdown() explicitamente se ele n�o tiver um.

    // Desligar ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    std::cout << "ImGuiManager encerrado." << std::endl;
}

void ImGuiManager::SetLunaStyle() {
    ImGuiStyle& style = ImGui::GetStyle();

    // Tema dark mais padr�o (similar ao "Dark" do ImGui, mas com pequenas altera��es)
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
    ImVec4 darkHeader = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    ImVec4 darkHeaderHovered = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    ImVec4 darkHeaderActive = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
    ImVec4 darkSeparator = darkBorder;
    ImVec4 darkResizeGrip = ImVec4(0.30f, 0.30f, 0.30f, 0.20f);
    ImVec4 darkResizeGripHovered = ImVec4(0.40f, 0.40f, 0.40f, 0.67f);
    ImVec4 darkResizeGripActive = ImVec4(0.50f, 0.50f, 0.50f, 0.95f);
    ImVec4 darkTab = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
    ImVec4 darkTabHovered = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    ImVec4 darkTabActive = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
    ImVec4 darkPlotLines = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    ImVec4 darkPlotLinesHovered = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    ImVec4 darkTextSelectedBg = ImVec4(0.40f, 0.40f, 0.40f, 0.35f);
    ImVec4 darkDragDropTarget = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    ImVec4 darkNavHighlight = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    ImVec4 darkDockingPreview = ImVec4(0.35f, 0.35f, 0.35f, 0.70f);
    ImVec4 darkDockingEmptyBg = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    ImVec4 darkModalWindowDimBg = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
    ImVec4 darkScrollbarBg = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    ImVec4 darkScrollbarGrab = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    ImVec4 darkScrollbarGrabHovered = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    ImVec4 darkScrollbarGrabActive = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    ImVec4 darkTabUnfocused = ImVec4(0.07f, 0.07f, 0.07f, 1.00f);
    ImVec4 darkTabUnfocusedActive = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);

    style.Colors[ImGuiCol_Text] = darkText;
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = darkBg;
    style.Colors[ImGuiCol_ChildBg] = darkBg;
    style.Colors[ImGuiCol_PopupBg] = darkBg;
    style.Colors[ImGuiCol_Border] = darkBorder;
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg] = darkFrameBg;
    style.Colors[ImGuiCol_FrameBgHovered] = darkFrameBgHovered;
    style.Colors[ImGuiCol_FrameBgActive] = darkFrameBgActive;
    style.Colors[ImGuiCol_TitleBg] = darkTitleBg;
    style.Colors[ImGuiCol_TitleBgActive] = darkTitleBgActive;
    style.Colors[ImGuiCol_TitleBgCollapsed] = darkTitleBg;
    style.Colors[ImGuiCol_MenuBarBg] = darkBg;
    style.Colors[ImGuiCol_ScrollbarBg] = darkScrollbarBg;
    style.Colors[ImGuiCol_ScrollbarGrab] = darkScrollbarGrab;
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = darkScrollbarGrabHovered;
    style.Colors[ImGuiCol_ScrollbarGrabActive] = darkScrollbarGrabActive;
    style.Colors[ImGuiCol_CheckMark] = darkCheckMark;
    style.Colors[ImGuiCol_SliderGrab] = darkSliderGrab;
    style.Colors[ImGuiCol_SliderGrabActive] = darkSliderGrabActive;
    style.Colors[ImGuiCol_Button] = darkButton;
    style.Colors[ImGuiCol_ButtonHovered] = darkButtonHovered;
    style.Colors[ImGuiCol_ButtonActive] = darkButtonActive;
    style.Colors[ImGuiCol_Header] = darkHeader;
    style.Colors[ImGuiCol_HeaderHovered] = darkHeaderHovered;
    style.Colors[ImGuiCol_HeaderActive] = darkHeaderActive;
    style.Colors[ImGuiCol_Separator] = darkSeparator;
    style.Colors[ImGuiCol_SeparatorHovered] = darkHeaderHovered;
    style.Colors[ImGuiCol_SeparatorActive] = darkHeaderActive;
    style.Colors[ImGuiCol_ResizeGrip] = darkResizeGrip;
    style.Colors[ImGuiCol_ResizeGripHovered] = darkResizeGripHovered;
    style.Colors[ImGuiCol_ResizeGripActive] = darkResizeGripActive;
    style.Colors[ImGuiCol_Tab] = darkTab;
    style.Colors[ImGuiCol_TabHovered] = darkTabHovered;
    style.Colors[ImGuiCol_TabActive] = darkTabActive;
    style.Colors[ImGuiCol_TabUnfocused] = darkTabUnfocused;
    style.Colors[ImGuiCol_TabUnfocusedActive] = darkTabUnfocusedActive;
    style.Colors[ImGuiCol_DockingPreview] = darkDockingPreview;
    style.Colors[ImGuiCol_DockingEmptyBg] = darkDockingEmptyBg;
    style.Colors[ImGuiCol_PlotLines] = darkPlotLines;
    style.Colors[ImGuiCol_PlotLinesHovered] = darkPlotLinesHovered;
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg] = darkTextSelectedBg;
    style.Colors[ImGuiCol_DragDropTarget] = darkDragDropTarget;
    style.Colors[ImGuiCol_NavHighlight] = darkNavHighlight;
    style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    style.Colors[ImGuiCol_ModalWindowDimBg] = darkModalWindowDimBg;

    style.WindowRounding = 0.0f;
    style.FrameRounding = 0.0f;
    style.GrabRounding = 0.0f;
    style.ScrollbarRounding = 0.0f;
    style.TabRounding = 0.0f;

    style.WindowPadding = ImVec2(6.0f, 6.0f);
    style.FramePadding = ImVec2(4.0f, 2.0f);
    style.ItemSpacing = ImVec2(6.0f, 4.0f);
    style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
    style.IndentSpacing = 20.0f;
    style.ScrollbarSize = 14.0f;
    style.GrabMinSize = 10.0f;
    style.ChildRounding = 0.0f;
}
void ImGuiManager::DrawAssetBrowser(bool& open)
{
    ImGui::Begin("Asset Browser", &open, ImGuiWindowFlags_NoScrollbar);
    ImGui::SetWindowSize(ImVec2(400, 600), ImGuiCond_FirstUseEver);

    // Popup de clique com o bot�o direito
    if (ImGui::BeginPopupContextWindow("AssetBrowserContextMenu"))
    {
        if (ImGui::MenuItem("Adicionar...")) { /* TODO */ }
        if (ImGui::MenuItem("Importar...")) { /* TODO */ }
        if (ImGui::MenuItem("Renomear")) { m_assetBrowserRenameActive = true; }
        if (ImGui::MenuItem("Criar Pasta...")) { m_assetBrowserCreateFolderActive = true; }
        ImGui::EndPopup();
    }

    // Barra de pesquisa
    if (!m_searchIconTexture)
    {
        m_searchIconTexture = std::make_shared<Texture>("C:/Users/josy-/Downloads/search.png");
        if (m_searchIconTexture->getID() == 0)
        {
            ImGui::Text("Erro ao carregar �cone de pesquisa.");
        }
    }

    if (m_searchIconTexture && m_searchIconTexture->getID() != 0)
    {
        ImVec2 iconSize = ImVec2(ImGui::GetTextLineHeight(), ImGui::GetTextLineHeight());
        ImGui::Image((ImTextureID)m_searchIconTexture->getID(), iconSize, ImVec2(0, 0), ImVec2(1, 1));
        ImGui::SameLine();
    }

    static char searchBuffer[256] = "";
    ImGui::InputTextWithHint("##Search", "Search...", searchBuffer, IM_ARRAYSIZE(searchBuffer));
    ImGui::Spacing();

    ImGui::Text("Assets");
    ImGui::Separator();

    // Simula��o da estrutura de diret�rios
    DrawDirectoryNode("Scenes", 0);
    DrawDirectoryNode("Tilesets", 0);
    DrawDirectoryNode("Sprites", 0);
    DrawDirectoryNode("UI", 0);
    DrawDirectoryNode("Audio", 0);

    // Input para renomear (modal)
    if (m_assetBrowserRenameActive)
    {
        ImGui::OpenPopup("Renomear");
        m_assetBrowserRenameActive = false;
    }
    if (ImGui::BeginPopupModal("Renomear", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        static char renameBuffer[256] = "";
        ImGui::InputText("Novo nome", renameBuffer, IM_ARRAYSIZE(renameBuffer));
        if (ImGui::Button("Renomear"))
        {
            std::cout << "Pedido de renomea��o para: " << renameBuffer << " (funcionalidade n�o implementada)\n";
            memset(renameBuffer, 0, sizeof(renameBuffer));
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancelar"))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    // Input para criar nova pasta (modal)
   // Input para criar nova pasta (modal)
    if (m_assetBrowserCreateFolderActive)
    {
        ImGui::OpenPopup("Criar Pasta");
        m_assetBrowserCreateFolderActive = false;
    }
    if (ImGui::BeginPopupModal("Criar Pasta", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        static char newFolderNameBuffer[256] = "";
        ImGui::InputText("Nome da pasta", newFolderNameBuffer, IM_ARRAYSIZE(newFolderNameBuffer));
        if (ImGui::Button("Criar"))
        {
            std::string newFolderName = newFolderNameBuffer;
            if (!newFolderName.empty())
            {
                std::cout << "Pedido de cria��o de pasta: " << newFolderName << " no caminho: " << m_assetBrowserCurrentPath << " (simulado)\n";
                // TODO: Aqui, em um sistema real, voc� criaria o diret�rio.
                // Para a simula��o, podemos adicionar essa pasta � lista retornada por GetDirectoryContents
                // Isso exigiria uma forma de armazenar e modificar essa estrutura simulada.
                // Por simplicidade agora, apenas vamos imprimir a inten��o.
                // Limpar o buffer
                memset(newFolderNameBuffer, 0, sizeof(newFolderNameBuffer));
                ImGui::CloseCurrentPopup();
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancelar"))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    ImGui::End();
}

void ImGuiManager::DrawDirectoryNode(const std::string& name, int depth)
{
    ImGui::PushID(name.c_str());
    std::string treeNodeLabel = name;
    for (int i = 0; i < depth; ++i)
    {
        treeNodeLabel = "  " + treeNodeLabel;
    }

    bool isDirectory = true; // Por enquanto, todos os n�s s�o tratados como diret�rios
    ImGuiTreeNodeFlags flags = isDirectory ? 0 : ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
    bool nodeOpen = ImGui::TreeNodeEx(treeNodeLabel.c_str(), flags);

    if (ImGui::IsItemClicked(0) && ImGui::IsMouseDoubleClicked(0))
    {
        std::cout << "Pasta/arquivo clicado duas vezes: " << name << " (navega��o simulada)\n";
        m_assetBrowserCurrentPath += "/" + name; // Simular navega��o
    }
    if (ImGui::IsItemClicked(1))
    {
        m_assetBrowserSelectedPath = m_assetBrowserCurrentPath + "/" + name;
        ImGui::OpenPopup("AssetBrowserContextMenu");
    }

    if (isDirectory && nodeOpen)
    {
        // Simular o conte�do de algumas pastas
        if (name == "Scenes") {
            DrawDirectoryNode("scene_01.scene", depth + 1);
            DrawDirectoryNode("level_02.scene", depth + 1);
        }
        else if (name == "Sprites") {
            DrawDirectoryNode("player.png", depth + 1);
            DrawDirectoryNode("enemy.png", depth + 1);
        }
        ImGui::TreePop();
    }
    ImGui::PopID();
}

std::vector<std::string> ImGuiManager::GetDirectoryContents(const std::string& path)
{
    // Simula��o: Retornar uma lista fixa de itens para diferentes "caminhos"
    if (path == "Assets") {
        return { "Scenes", "Tilesets", "Sprites", "UI", "Audio" };
    }
    else if (path == "Assets/Scenes") {
        return { "scene_01.scene", "level_02.scene" };
    }
    else if (path == "Assets/Sprites") {
        return { "player.png", "enemy.png" };
    }
    return {};
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

void ImGuiManager::DrawEditorUI(float deltaTime) {
    // DockSpace est� ativo no BeginFrame/EndFrame
    static bool assetBrowserOpen = true;
    DrawAssetBrowser(assetBrowserOpen);
   /* if (m_showViewport) {
        ImGui::Begin("Viewport");
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        ImGui::Image(m_renderTarget.getTextureID(), viewportPanelSize, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();
    }*/

    if (m_showSpritesheetEditor) {
        m_spritesheetEditor.DrawUI(deltaTime);
    }


  //  this->assetBrowser.render();
}