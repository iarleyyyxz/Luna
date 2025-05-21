#ifndef SPRITESHEET_EDITOR_HPP
#define SPRITESHEET_EDITOR_HPP

#include <string>
#include <vector>
#include <glm.hpp> // Para glm::vec2, glm::vec4

#include "Spritesheet.hpp" // Para carregar e gerenciar spritesheets
#include "Sprite.hpp"      // Para exibir sprites individuais
#include "Anim/Animation.hpp"   // Para exibir a anima��o

// NOVO: Incluir a biblioteca ImGuiFileDialog
#include "ImGuiFileDialog.h"

// Forward declarations para evitar inclus�es desnecess�rias
class Renderer2D;
class Keyboard;
class Mouse;

class SpritesheetEditor {
public:
    SpritesheetEditor();
    ~SpritesheetEditor();

    // Inicializa o editor, recebendo refer�ncias aos recursos da engine
    void Init(Renderer2D& renderer, Keyboard& keyboard, Mouse& mouse);

    // Desenha a interface do utilizador do editor usando ImGui
    // deltaTime: Necess�rio para atualizar a anima��o de pr�-visualiza��o
    void DrawUI(float deltaTime);

    // Carrega uma nova spritesheet para edi��o
    bool LoadSpritesheet(const std::string& filePath, int frameWidth, int frameHeight);

private:
    Renderer2D* m_renderer; // Refer�ncia ao Renderer2D para desenhar a pr�-visualiza��o
    Keyboard* m_keyboard;   // Refer�ncia ao Keyboard (se o editor precisar de input direto)
    Mouse* m_mouse;         // Refer�ncia ao Mouse (se o editor precisar de input direto)

    Spritesheet* m_currentSpritesheet; // A spritesheet que est� sendo editada
    Sprite* m_previewSprite;           // O sprite atualmente selecionado para pr�-visualiza��o
    Animation* m_previewAnimation;     // A anima��o de pr�-visualiza��o

    // Par�metros para sele��o de frame e anima��o
    int m_selectedCol;
    int m_selectedRow;
    int m_animationStartCol;
    int m_animationStartRow;
    int m_animationEndCol;
    int m_animationEndRow;
    float m_animationSpeed; // Dura��o de cada frame da anima��o em segundos

    std::string m_spritesheetPath; // Caminho do arquivo da spritesheet carregada
    char m_filePathBuffer[256];    // Buffer para input de caminho de arquivo no ImGui
    int m_frameWidthInput;         // Input para largura do frame
    int m_frameHeightInput;        // Input para altura do frame

    // REMOVIDO: m_showFileDialog e m_dialogFilePathBuffer
    // bool m_showFileDialog;
    // char m_dialogFilePathBuffer[256];

    // NOVO: Inst�ncia do ImGuiFileDialog
    ImGuiFileDialog m_fileDialog;

    // Fun��es auxiliares
    void updatePreviewSprite();
    void updatePreviewAnimation();
    // REMOVIDO: drawFileDialog()
    // void drawFileDialog();
};

#endif // SPRITESHEET_EDITOR_HPP
