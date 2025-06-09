#pragma once

#include "Source/Ecs/SceneObject.hpp"    // Inclui a defini��o de SceneObject
#include "Source/Ecs/Transform2D.hpp"    // Inclui a defini��o de Transform2D (se n�o inclu�do por SceneObject)
#include "Source/Graphics/Renderer/Renderer2D.hpp" // Inclui a defini��o de Renderer2D
#include "Source/Core/Camera2D.hpp"        // Inclui a defini��o de Camera (assumindo que voc� tem uma)

#include <glm.hpp>
 // Para glm::rotate

namespace Luna {
    namespace Editor {

        class Gizmo2D {
        public:
            Gizmo2D() : m_position(0.0f), m_radius(20.0f) {} // Default values
            ~Gizmo2D() = default;

            // Define o SceneObject atualmente selecionado para o gizmo manipular.
            // Recebe um ponteiro para SceneObject.
            void SetSelectedObject(SceneObject* object);

            // Atualiza o estado do gizmo (detec��o de clique, arrastar).
            // mouseScreenX/Y: Posi��o do mouse em coordenadas de tela (pixels).
            // mouseDown: Se o bot�o esquerdo do mouse est� pressionado.
            // camera: A c�mera atual da cena para convers�es de coordenadas.
            void Update(float mouseScreenX, float mouseScreenY, bool mouseDown, const Camera2D& camera);

            // Renderiza o gizmo na tela.
            // renderer: O renderizador 2D da engine.
            // camera: A c�mera atual da cena para convers�es de coordenadas.
            void Render(Renderer2D& renderer, const Camera2D& camera);

            void SetPosition(const glm::vec2& pos) { m_position = pos; }
            glm::vec2 GetPosition() const { return m_position; }
            float GetRadius() const { return m_radius; } // Retorna um raio de exemplo para detec��o de colis�o

        private:
            glm::vec2 m_position;
            float m_radius; // Usado para detec��o de clique

            SceneObject* m_selectedObject = nullptr; // Ponteiro para o SceneObject selecionado

            bool m_isDragging = false; // Se o gizmo est� sendo arrastado
            enum class GizmoHandle {
                None,
                TranslateX,  // Eixo X para transla��o
                TranslateY,  // Eixo Y para transla��o
                // Futuras al�as para escala e rota��o podem ser adicionadas aqui
                // Scale,
                // Rotate
            };
            GizmoHandle m_activeHandle = GizmoHandle::None; // O handle que est� sendo arrastado

            // Posi��es e estados salvos ao iniciar um arrasto (em coordenadas de mundo)
            glm::vec2 m_dragStartMouseWorldPos;  // Posi��o do mouse no mundo no in�cio do arrasto
            glm::vec2 m_dragStartObjectWorldPos; // Posi��o do objeto no mundo no in�cio do arrasto

            // Constantes de estilo do gizmo (em pixels de tela)
            const float TRANSLATE_AXIS_LENGTH_SCREEN_PIXELS = 60.0f;
            const float TRANSLATE_AXIS_THICKNESS_SCREEN_PIXELS = 4.0f;
            const float TRANSLATE_ARROW_SIZE_SCREEN_PIXELS = 10.0f; // Tamanho da ponta da seta

            // Cores do Gizmo
            const glm::vec4 COLOR_TRANSLATE_X = glm::vec4(1.0f, 0.2f, 0.2f, 1.0f); // Vermelho
            const glm::vec4 COLOR_TRANSLATE_Y = glm::vec4(0.2f, 1.0f, 0.2f, 1.0f); // Verde
            const glm::vec4 COLOR_ACTIVE = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);    // Branco (quando ativo)

            // --- M�todos de Convers�o de Coordenadas ---
            // Converte uma coordenada de mundo para uma coordenada de tela (pixels).
            glm::vec2 WorldToScreen(glm::vec2 worldPos, const Camera2D& camera) const;
            // Converte uma coordenada de tela (pixels) para uma coordenada de mundo.
            glm::vec2 ScreenToWorld(glm::vec2 screenPos, const Camera2D& camera) const;
            // Calcula o tamanho em mundo de um tamanho em pixels na tela (para manter o gizmo constante).
            float ScreenSizeToWorldSize(float screenPixels, const Camera2D& camera) const;

            // --- M�todos de Detec��o de Colis�o (Picking) ---
            // Verifica se um ponto (mouse) est� sobre um segmento de linha rotacionado.
            // Retorna true se houver interse��o dentro de uma toler�ncia.
            bool IsMouseOverLineSegment(glm::vec2 mousePosScreen, glm::vec2 p1Screen, glm::vec2 p2Screen, float toleranceScreen) const;
        };

    } // namespace Editor
} // namespace Luna