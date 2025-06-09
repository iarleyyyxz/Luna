#pragma once

#include "Source/Ecs/SceneObject.hpp"    // Inclui a definição de SceneObject
#include "Source/Ecs/Transform2D.hpp"    // Inclui a definição de Transform2D (se não incluído por SceneObject)
#include "Source/Graphics/Renderer/Renderer2D.hpp" // Inclui a definição de Renderer2D
#include "Source/Core/Camera2D.hpp"        // Inclui a definição de Camera (assumindo que você tem uma)

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

            // Atualiza o estado do gizmo (detecção de clique, arrastar).
            // mouseScreenX/Y: Posição do mouse em coordenadas de tela (pixels).
            // mouseDown: Se o botão esquerdo do mouse está pressionado.
            // camera: A câmera atual da cena para conversões de coordenadas.
            void Update(float mouseScreenX, float mouseScreenY, bool mouseDown, const Camera2D& camera);

            // Renderiza o gizmo na tela.
            // renderer: O renderizador 2D da engine.
            // camera: A câmera atual da cena para conversões de coordenadas.
            void Render(Renderer2D& renderer, const Camera2D& camera);

            void SetPosition(const glm::vec2& pos) { m_position = pos; }
            glm::vec2 GetPosition() const { return m_position; }
            float GetRadius() const { return m_radius; } // Retorna um raio de exemplo para detecção de colisão

        private:
            glm::vec2 m_position;
            float m_radius; // Usado para detecção de clique

            SceneObject* m_selectedObject = nullptr; // Ponteiro para o SceneObject selecionado

            bool m_isDragging = false; // Se o gizmo está sendo arrastado
            enum class GizmoHandle {
                None,
                TranslateX,  // Eixo X para translação
                TranslateY,  // Eixo Y para translação
                // Futuras alças para escala e rotação podem ser adicionadas aqui
                // Scale,
                // Rotate
            };
            GizmoHandle m_activeHandle = GizmoHandle::None; // O handle que está sendo arrastado

            // Posições e estados salvos ao iniciar um arrasto (em coordenadas de mundo)
            glm::vec2 m_dragStartMouseWorldPos;  // Posição do mouse no mundo no início do arrasto
            glm::vec2 m_dragStartObjectWorldPos; // Posição do objeto no mundo no início do arrasto

            // Constantes de estilo do gizmo (em pixels de tela)
            const float TRANSLATE_AXIS_LENGTH_SCREEN_PIXELS = 60.0f;
            const float TRANSLATE_AXIS_THICKNESS_SCREEN_PIXELS = 4.0f;
            const float TRANSLATE_ARROW_SIZE_SCREEN_PIXELS = 10.0f; // Tamanho da ponta da seta

            // Cores do Gizmo
            const glm::vec4 COLOR_TRANSLATE_X = glm::vec4(1.0f, 0.2f, 0.2f, 1.0f); // Vermelho
            const glm::vec4 COLOR_TRANSLATE_Y = glm::vec4(0.2f, 1.0f, 0.2f, 1.0f); // Verde
            const glm::vec4 COLOR_ACTIVE = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);    // Branco (quando ativo)

            // --- Métodos de Conversão de Coordenadas ---
            // Converte uma coordenada de mundo para uma coordenada de tela (pixels).
            glm::vec2 WorldToScreen(glm::vec2 worldPos, const Camera2D& camera) const;
            // Converte uma coordenada de tela (pixels) para uma coordenada de mundo.
            glm::vec2 ScreenToWorld(glm::vec2 screenPos, const Camera2D& camera) const;
            // Calcula o tamanho em mundo de um tamanho em pixels na tela (para manter o gizmo constante).
            float ScreenSizeToWorldSize(float screenPixels, const Camera2D& camera) const;

            // --- Métodos de Detecção de Colisão (Picking) ---
            // Verifica se um ponto (mouse) está sobre um segmento de linha rotacionado.
            // Retorna true se houver interseção dentro de uma tolerância.
            bool IsMouseOverLineSegment(glm::vec2 mousePosScreen, glm::vec2 p1Screen, glm::vec2 p2Screen, float toleranceScreen) const;
        };

    } // namespace Editor
} // namespace Luna