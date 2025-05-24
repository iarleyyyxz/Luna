#define _CRT_SECURE_NO_WARNINGS // Para desativar o aviso C4996 para strcpy, se preferir mantê-lo.
// No entanto, é melhor usar strcpy_s.

#include "SpritesheetEditor.hpp"
#include <iostream>
#include "imgui.h" // Para usar ImGui
#include "Renderer2D.hpp" // Para usar Renderer2D
#include "InputManager.hpp" // Para usar Keyboard e Mouse
#include <cstring> // Para strcpy_s
#include <algorithm> // Para std::min, std::max, std::swap

// NOVO: Incluir a biblioteca ImGuiFileDialog
#include "ImGuiFileDialog.h"

SpritesheetEditor::SpritesheetEditor()
    : m_renderer(nullptr), m_keyboard(nullptr), m_mouse(nullptr),
    m_currentSpritesheet(nullptr), m_previewSprite(nullptr), m_previewAnimation(nullptr),
    m_selectedCol(0), m_selectedRow(0),
    m_animationStartCol(0), m_animationStartRow(0),
    m_animationEndCol(0), m_animationEndRow(0),
    m_animationSpeed(0.15f), // Velocidade padrão da animação
    m_frameWidthInput(32), m_frameHeightInput(32) // Valores padrão para input
    // REMOVIDO: m_showFileDialog(false)
    // REMOVIDO: m_dialogFilePathBuffer
{
    // Inicializa o buffer do caminho do arquivo com um valor padrão
    // Usando strcpy_s para uma versão mais segura de strcpy
    strcpy_s(m_filePathBuffer, IM_ARRAYSIZE(m_filePathBuffer), "Resources/spritesheet_example.png"); // Exemplo de caminho padrão
    // REMOVIDO: strcpy_s(m_dialogFilePathBuffer, IM_ARRAYSIZE(m_dialogFilePathBuffer), "");
    std::cout << "SpritesheetEditor construído!" << std::endl;
}

SpritesheetEditor::~SpritesheetEditor() {
    // Libera os recursos
    if (m_previewAnimation) {
        delete m_previewAnimation;
        m_previewAnimation = nullptr;
    }
    if (m_previewSprite) {
        delete m_previewSprite;
        m_previewSprite = nullptr;
    }
    if (m_currentSpritesheet) {
        delete m_currentSpritesheet; // Isso também deletará a textura da spritesheet
        m_currentSpritesheet = nullptr;
    }
    std::cout << "SpritesheetEditor destruído!" << std::endl;
}

void SpritesheetEditor::Init(Renderer2D& renderer, Keyboard& keyboard, Mouse& mouse) {
    m_renderer = &renderer;
    m_keyboard = &keyboard;
    m_mouse = &mouse;
    std::cout << "SpritesheetEditor inicializado." << std::endl;
}

bool SpritesheetEditor::LoadSpritesheet(const std::string& filePath, int frameWidth, int frameHeight) {
    // Limpa a spritesheet anterior e os sprites/animações associados
    if (m_previewAnimation) {
        delete m_previewAnimation;
        m_previewAnimation = nullptr;
    }
    if (m_previewSprite) {
        delete m_previewSprite;
        m_previewSprite = nullptr;
    }
    if (m_currentSpritesheet) {
        delete m_currentSpritesheet;
        m_currentSpritesheet = nullptr;
    }

    m_currentSpritesheet = new Spritesheet(filePath, frameWidth, frameHeight);
    if (!m_currentSpritesheet->isValid()) {
        std::cerr << "ERRO::SPRITESHEET_EDITOR: Falha ao carregar spritesheet: " << filePath << std::endl;
        delete m_currentSpritesheet;
        m_currentSpritesheet = nullptr;
        return false;
    }

    m_spritesheetPath = filePath;
    m_frameWidthInput = frameWidth;
    m_frameHeightInput = frameHeight;

    // Redefine a seleção para o primeiro frame
    m_selectedCol = 0;
    m_selectedRow = 0;
    m_animationStartCol = 0;
    m_animationStartRow = 0;
    m_animationEndCol = 0;
    m_animationEndRow = 0;

    updatePreviewSprite(); // Atualiza o sprite de pré-visualização para o primeiro frame
    updatePreviewAnimation(); // Atualiza a animação de pré-visualização

    std::cout << "Spritesheet '" << filePath << "' carregada com sucesso no editor." << std::endl;
    return true;
}

void SpritesheetEditor::updatePreviewSprite() {
    if (m_currentSpritesheet) {
        if (m_previewSprite) {
            delete m_previewSprite;
            m_previewSprite = nullptr;
        }
        m_previewSprite = m_currentSpritesheet->getSprite(m_selectedCol, m_selectedRow);
    }
}

void SpritesheetEditor::updatePreviewAnimation() {
    if (m_currentSpritesheet) {
        if (m_previewAnimation) {
            delete m_previewAnimation;
            m_previewAnimation = nullptr;
        }

        std::vector<Sprite*> frames;
        int maxCols = m_currentSpritesheet->getTexture()->width / m_frameWidthInput;
        int maxRows = m_currentSpritesheet->getTexture()->height / m_frameHeightInput;

        // Clampar os valores de início e fim para garantir que estejam dentro dos limites da spritesheet
        int actualStartCol = std::clamp(m_animationStartCol, 0, maxCols - 1);
        int actualStartRow = std::clamp(m_animationStartRow, 0, maxRows - 1);
        int actualEndCol = std::clamp(m_animationEndCol, 0, maxCols - 1);
        int actualEndRow = std::clamp(m_animationEndRow, 0, maxRows - 1);

        // Garante que o fim não seja menor que o início para iteração linear
        if (actualStartRow > actualEndRow) std::swap(actualStartRow, actualEndRow);
        // Se a linha for a mesma, mas a coluna final for menor que a inicial, inverte
        if (actualStartRow == actualEndRow && actualStartCol > actualEndCol) std::swap(actualStartCol, actualEndCol);


        for (int r = actualStartRow; r <= actualEndRow; ++r) {
            int startC = (r == actualStartRow) ? actualStartCol : 0;
            int endC = (r == actualEndRow) ? actualEndCol : maxCols - 1;

            // Garante que a iteração de coluna seja válida e dentro dos limites
            startC = std::clamp(startC, 0, maxCols - 1);
            endC = std::clamp(endC, 0, maxCols - 1);

            for (int c = startC; c <= endC; ++c) {
                Sprite* frame = m_currentSpritesheet->getSprite(c, r);
                if (frame) {
                    frames.push_back(frame);
                }
            }
        }

        if (!frames.empty()) {
            m_previewAnimation = new Animation(frames, m_animationSpeed);
            m_previewAnimation->SetLooping(true);
            m_previewAnimation->Reset();
        }
        else {
            std::cerr << "AVISO::SPRITESHEET_EDITOR: Nenhuns frames encontrados para a animação de pré-visualização. Verifique os limites." << std::endl;
        }
    }
}

// REMOVIDO: drawFileDialog() não é mais necessário, ImGuiFileDialog gerencia seu próprio desenho.
// void SpritesheetEditor::drawFileDialog() { ... }


void SpritesheetEditor::DrawUI(float deltaTime) {
   

    ImGui::Begin("Editor de Spritesheets");

    // === Seção de Carregamento da Spritesheet (mantida como CollapsingHeader para organização) ===
    if (ImGui::CollapsingHeader("Carregar Spritesheet", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::InputText("Caminho da Spritesheet", m_filePathBuffer, IM_ARRAYSIZE(m_filePathBuffer));
        ImGui::SameLine(); // Coloca o botão na mesma linha
        if (ImGui::Button("Procurar...")) { // Botão para abrir o diálogo de ficheiros
            // NOVO: Abre o diálogo de ficheiros
            m_fileDialog.OpenDialog("ChooseFileDlgKey", "Selecionar Spritesheet", ".png,.jpg,.jpeg");
        }

        ImGui::InputInt("Largura do Frame", &m_frameWidthInput);
        ImGui::InputInt("Altura do Frame", &m_frameHeightInput);

        if (ImGui::Button("Carregar")) {
            LoadSpritesheet(m_filePathBuffer, m_frameWidthInput, m_frameHeightInput);
        }

        if (m_currentSpritesheet) {
            ImGui::Text("Spritesheet Carregada: %s", m_spritesheetPath.c_str());
            ImGui::Text("Dimensões: %dx%d", m_currentSpritesheet->getTexture()->width, m_currentSpritesheet->getTexture()->height);
            ImGui::Text("Frames por Linha/Coluna: %dx%d", m_currentSpritesheet->getTexture()->width / m_frameWidthInput, m_currentSpritesheet->getTexture()->height / m_frameHeightInput);
        }
        else {
            ImGui::Text("Nenhuma spritesheet carregada.");
        }
        ImGui::Spacing();
    }

    if (m_currentSpritesheet) {
        // === Layout Principal: Pré-visualização à Esquerda, Controles à Direita ===
        // Definir a largura da coluna da pré-visualização (ex: 60% da largura da janela)
        float previewPanelWidth = ImGui::GetContentRegionAvail().x * 0.6f;
        float controlsPanelWidth = ImGui::GetContentRegionAvail().x - previewPanelWidth - ImGui::GetStyle().ItemSpacing.x;

        // Painel Esquerdo: Pré-visualização da Animação
        ImGui::BeginChild("##AnimationPreviewPanel", ImVec2(previewPanelWidth, 0), true); // 0 para altura automática
        ImGui::Text("Pré-visualização da Animação:");
        ImGui::Separator();

        if (m_previewAnimation && m_renderer) {
            m_previewAnimation->Update(deltaTime); // Atualiza a animação
            Sprite* currentFrame = m_previewAnimation->GetCurrentFrame();
            if (currentFrame) {
                // Calcular o tamanho da pré-visualização para preencher o espaço disponível
                float maxPreviewDim = std::min(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
                ImVec2 previewSize = ImVec2(maxPreviewDim, maxPreviewDim);

                // Centralizar a imagem na área de pré-visualização
                ImVec2 cursor = ImGui::GetCursorPos();
                ImVec2 contentRegion = ImGui::GetContentRegionAvail();
                float xOffset = (contentRegion.x - previewSize.x) * 0.5f;
                float yOffset = (contentRegion.y - previewSize.y) * 0.5f;
                ImGui::SetCursorPos(ImVec2(cursor.x + xOffset, cursor.y + yOffset));

                ImGui::Image(currentFrame->getTexture()->getID(),
                    previewSize,
                    ImVec2(currentFrame->getTexCoordMin().x, currentFrame->getTexCoordMax().y), // UVs corrigidas para ImGui
                    ImVec2(currentFrame->getTexCoordMax().x, currentFrame->getTexCoordMin().y));
            }
        }
        else {
            ImGui::Text("Nenhuma animação para pré-visualizar.");
        }
        ImGui::EndChild(); // Fim do painel de pré-visualização

        ImGui::SameLine(); // Colocar o próximo item na mesma linha

        // Painel Direito: Controles
        ImGui::BeginGroup(); // Agrupar os controles do lado direito

        // === Controles de Animação ===
        ImGui::Text("Configuração da Animação:");
        ImGui::PushItemWidth(controlsPanelWidth * 0.45f); // Ajustar largura dos inputs

        ImGui::Text("Início:"); ImGui::SameLine();
        ImGui::InputInt("Col##Start", &m_animationStartCol); ImGui::SameLine();
        ImGui::InputInt("Lin##Start", &m_animationStartRow);

        ImGui::Text("Fim:"); ImGui::SameLine();
        ImGui::InputInt("Col##End", &m_animationEndCol); ImGui::SameLine();
        ImGui::InputInt("Lin##End", &m_animationEndRow);
        ImGui::PopItemWidth(); // Voltar à largura padrão

        // Clampar os valores para evitar índices fora dos limites
        int maxCols = m_currentSpritesheet->getTexture()->width / m_frameWidthInput - 1;
        int maxRows = m_currentSpritesheet->getTexture()->height / m_frameHeightInput - 1;
        m_animationStartCol = std::clamp(m_animationStartCol, 0, maxCols);
        m_animationEndCol = std::clamp(m_animationEndCol, 0, maxCols);
        m_animationStartRow = std::clamp(m_animationStartRow, 0, maxRows);
        m_animationEndRow = std::clamp(m_animationEndRow, 0, maxRows);

        ImGui::SliderFloat("Velocidade (seg/frame)", &m_animationSpeed, 0.01f, 1.0f, "%.2f");

        if (ImGui::Button("Atualizar Animação", ImVec2(controlsPanelWidth, 0))) { // Botão de largura total do painel
            updatePreviewAnimation();
        }
        ImGui::Spacing();

        // === Seleção de Frame Individual (abaixo dos controles de animação) ===
        ImGui::Text("Seleção de Frame Individual:");
        ImGui::PushItemWidth(controlsPanelWidth * 0.45f);
        if (ImGui::SliderInt("Coluna", &m_selectedCol, 0, maxCols)) {
            updatePreviewSprite();
        }
        if (ImGui::SliderInt("Linha", &m_selectedRow, 0, maxRows)) {
            updatePreviewSprite();
        }
        ImGui::PopItemWidth();

        // Pré-visualização do sprite individual (menor, abaixo dos sliders)
        if (m_previewSprite && m_renderer) {
            ImGui::Text("Pré-visualização do Sprite:");
            float individualPreviewScale = 2.0f; // Escala menor para o sprite individual
            ImVec2 individualPreviewSize = ImVec2(static_cast<float>(m_frameWidthInput) * individualPreviewScale, static_cast<float>(m_frameHeightInput) * individualPreviewScale);
            ImGui::Image(m_previewSprite->getTexture()->getID(),
                individualPreviewSize,
                ImVec2(m_previewSprite->getTexCoordMin().x, m_previewSprite->getTexCoordMax().y), // UVs corrigidas para ImGui
                ImVec2(m_previewSprite->getTexCoordMax().x, m_previewSprite->getTexCoordMin().y));
        }

        ImGui::EndGroup(); // Fim do grupo de controles

    } // Fim do if (m_currentSpritesheet)

    ImGui::End(); // Fim da janela "Editor de Spritesheets"

    // NOVO: Desenha o diálogo de ficheiros do ImGuiFileDialog
    if (m_fileDialog.Display("ChooseFileDlgKey")) {
        if (m_fileDialog.IsOk()) { // Use IsOk() para verificar se o utilizador clicou OK
            std::string filePath = m_fileDialog.GetFilePathName();
            strcpy_s(m_filePathBuffer, IM_ARRAYSIZE(m_filePathBuffer), filePath.c_str());
            // Opcional: Carregar a spritesheet imediatamente após a seleção
            // LoadSpritesheet(filePath, m_frameWidthInput, m_frameHeightInput);
        }
        m_fileDialog.Close();
    }
}