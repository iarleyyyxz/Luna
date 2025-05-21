#ifndef SPRITESHEET_EDITOR_HPP
#define SPRITESHEET_EDITOR_HPP

#include <string>
#include <vector>
#include <glm.hpp> // Para glm::vec2, glm::vec4

#include "Spritesheet.hpp" // Para carregar e gerenciar spritesheets
#include "Sprite.hpp"      // Para exibir sprites individuais
#include "Anim/Animation.hpp"   // Para exibir a animação

// NOVO: Incluir a biblioteca ImGuiFileDialog
#include "ImGuiFileDialog.h"

// Forward declarations para evitar inclusões desnecessárias
class Renderer2D;
class Keyboard;
class Mouse;

class SpritesheetEditor {
public:
    SpritesheetEditor();
    ~SpritesheetEditor();

    // Inicializa o editor, recebendo referências aos recursos da engine
    void Init(Renderer2D& renderer, Keyboard& keyboard, Mouse& mouse);

    // Desenha a interface do utilizador do editor usando ImGui
    // deltaTime: Necessário para atualizar a animação de pré-visualização
    void DrawUI(float deltaTime);

    // Carrega uma nova spritesheet para edição
    bool LoadSpritesheet(const std::string& filePath, int frameWidth, int frameHeight);

private:
    Renderer2D* m_renderer; // Referência ao Renderer2D para desenhar a pré-visualização
    Keyboard* m_keyboard;   // Referência ao Keyboard (se o editor precisar de input direto)
    Mouse* m_mouse;         // Referência ao Mouse (se o editor precisar de input direto)

    Spritesheet* m_currentSpritesheet; // A spritesheet que está sendo editada
    Sprite* m_previewSprite;           // O sprite atualmente selecionado para pré-visualização
    Animation* m_previewAnimation;     // A animação de pré-visualização

    // Parâmetros para seleção de frame e animação
    int m_selectedCol;
    int m_selectedRow;
    int m_animationStartCol;
    int m_animationStartRow;
    int m_animationEndCol;
    int m_animationEndRow;
    float m_animationSpeed; // Duração de cada frame da animação em segundos

    std::string m_spritesheetPath; // Caminho do arquivo da spritesheet carregada
    char m_filePathBuffer[256];    // Buffer para input de caminho de arquivo no ImGui
    int m_frameWidthInput;         // Input para largura do frame
    int m_frameHeightInput;        // Input para altura do frame

    // REMOVIDO: m_showFileDialog e m_dialogFilePathBuffer
    // bool m_showFileDialog;
    // char m_dialogFilePathBuffer[256];

    // NOVO: Instância do ImGuiFileDialog
    ImGuiFileDialog m_fileDialog;

    // Funções auxiliares
    void updatePreviewSprite();
    void updatePreviewAnimation();
    // REMOVIDO: drawFileDialog()
    // void drawFileDialog();
};

#endif // SPRITESHEET_EDITOR_HPP
