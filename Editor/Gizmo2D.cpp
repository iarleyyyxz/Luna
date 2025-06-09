#include "Gizmo2D.hpp"
#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL

#include <gtx/norm.hpp>        // Para glm::distance2
#include <rotate_vector.hpp> // <-- Inclua esta linha aqui e no .hpp
#include <algorithm>               // Para glm::clamp

namespace Luna {
    namespace Editor {

        void Gizmo2D::SetSelectedObject(SceneObject* object) {
            m_selectedObject = object;
            m_isDragging = false;
            m_activeHandle = GizmoHandle::None;
        }

        glm::vec2 Gizmo2D::WorldToScreen(glm::vec2 worldPos, const Camera2D& camera) const {
            // 1. Converter para coordenadas relativas à câmera (mundo)
            glm::vec2 cameraRelativeWorldPos = worldPos - camera.getPosition();

            // 2. Aplicar zoom
            cameraRelativeWorldPos *= camera.getZoom();

            // 3. Converter para coordenadas de tela (pixels, com (0,0) no centro da viewport)
            glm::vec2 screenPosCentered = cameraRelativeWorldPos;

            // 4. Mover para o canto superior esquerdo (assumindo (0,0) no canto superior esquerdo para a tela)
            return screenPosCentered + glm::vec2(camera.getViewportWidth() * 0.5f, camera.getViewportHeight() * 0.5f);
        }

        glm::vec2 Gizmo2D::ScreenToWorld(glm::vec2 screenPos, const Camera2D& camera) const {
            // 1. Mover para (0,0) no centro da tela
            glm::vec2 screenPosCentered = screenPos - glm::vec2(camera.getViewportWidth() * 0.5f, camera.getViewportHeight() * 0.5f);

            // 2. Remover zoom
            glm::vec2 cameraRelativeWorldPos = screenPosCentered / camera.getZoom();

            // 3. Adicionar posição da câmera para obter coordenadas de mundo
            return cameraRelativeWorldPos + camera.getPosition();
        }

        float Gizmo2D::ScreenSizeToWorldSize(float screenPixels, const Camera2D& camera) const {
            return screenPixels / camera.getZoom();
        }

        // Verifica se um ponto está próximo a um segmento de linha (usado para picking de eixos)
        bool Gizmo2D::IsMouseOverLineSegment(glm::vec2 mousePosScreen, glm::vec2 p1Screen, glm::vec2 p2Screen, float toleranceScreen) const {
            float lengthSquared = glm::distance2(p1Screen, p2Screen);
            if (lengthSquared == 0.0f) { // Ponto degenerado
                return glm::distance2(mousePosScreen, p1Screen) < (toleranceScreen * toleranceScreen);
            }

            // Projeta o ponto do mouse no segmento de linha
            float t = glm::dot(mousePosScreen - p1Screen, p2Screen - p1Screen) / lengthSquared;
            t = glm::clamp(t, 0.0f, 1.0f); // Clampa para o segmento de linha (0 a 1, indicando onde na linha)

            glm::vec2 projection = p1Screen + t * (p2Screen - p1Screen);

            // Verifica a distância do ponto do mouse à projeção
            return glm::distance2(mousePosScreen, projection) < (toleranceScreen * toleranceScreen);
        }


        void Gizmo2D::Update(float mouseScreenX, float mouseScreenY, bool mouseDown, const Camera2D& camera) {
            if (!m_selectedObject) {
                m_isDragging = false;
                m_activeHandle = GizmoHandle::None;
                return;
            }

            // Obtenha a Transform2D do objeto selecionado
            Transform2D& objTransform = m_selectedObject->GetTransform();

            glm::vec2 mouseScreenPos = glm::vec2(mouseScreenX, mouseScreenY);

            // As alças do gizmo devem ter um tamanho consistente na tela,
            // então calculamos seus tamanhos em mundo com base no zoom da câmera.
            float axisLengthWorld = ScreenSizeToWorldSize(TRANSLATE_AXIS_LENGTH_SCREEN_PIXELS, camera);
            float axisThicknessWorld = ScreenSizeToWorldSize(TRANSLATE_AXIS_THICKNESS_SCREEN_PIXELS, camera);
            float arrowSizeWorld = ScreenSizeToWorldSize(TRANSLATE_ARROW_SIZE_SCREEN_PIXELS, camera);

            // Calcule as posições dos eixos em coordenadas de mundo, considerando a rotação do objeto
            // Usando rotation.z que é um float (radianos)
            glm::vec2 xAxisDir = glm::rotate(glm::vec2(1.0f, 0.0f), objTransform.rotation.z);
            glm::vec2 yAxisDir = glm::rotate(glm::vec2(0.0f, 1.0f), objTransform.rotation.z);

            glm::vec2 xAxisEndWorld = objTransform.position + xAxisDir * axisLengthWorld;
            glm::vec2 yAxisEndWorld = objTransform.position + yAxisDir * axisLengthWorld;

            // Converta para coordenadas de tela para picking
            glm::vec2 xAxisStartScreen = WorldToScreen(objTransform.position, camera);
            glm::vec2 xAxisEndScreen = WorldToScreen(xAxisEndWorld, camera);
            glm::vec2 yAxisStartScreen = WorldToScreen(objTransform.position, camera);
            glm::vec2 yAxisEndScreen = WorldToScreen(yAxisEndWorld, camera);

            // Picking logic
            if (mouseDown && !m_isDragging) {
                // Tolerância para o picking dos eixos (em pixels de tela)
                float pickingTolerance = TRANSLATE_AXIS_THICKNESS_SCREEN_PIXELS * 1.5f;

                if (IsMouseOverLineSegment(mouseScreenPos, xAxisStartScreen, xAxisEndScreen, pickingTolerance)) {
                    m_activeHandle = GizmoHandle::TranslateX;
                }
                else if (IsMouseOverLineSegment(mouseScreenPos, yAxisStartScreen, yAxisEndScreen, pickingTolerance)) {
                    m_activeHandle = GizmoHandle::TranslateY;
                }

                if (m_activeHandle != GizmoHandle::None) {
                    m_isDragging = true;
                    m_dragStartMouseWorldPos = ScreenToWorld(mouseScreenPos, camera); // Salva a posição do mouse no mundo
                    m_dragStartObjectWorldPos = objTransform.position; // Salva a posição inicial do objeto
                }
            }
            else if (!mouseDown && m_isDragging) { // Mouse foi solto, parar arrastar
                m_isDragging = false;
                m_activeHandle = GizmoHandle::None;
            }

            // Dragging logic
            if (m_isDragging && m_selectedObject) {
                glm::vec2 currentMouseWorldPos = ScreenToWorld(mouseScreenPos, camera);
                glm::vec2 deltaWorld = currentMouseWorldPos - m_dragStartMouseWorldPos;

                switch (m_activeHandle) {
                case GizmoHandle::TranslateX: {
                    // Projeta o delta do mouse no eixo X local do objeto
                    float dotProduct = glm::dot(deltaWorld, xAxisDir);
                    objTransform.position = m_dragStartObjectWorldPos + xAxisDir * dotProduct;
                    break;
                }
                case GizmoHandle::TranslateY: {
                    // Projeta o delta do mouse no eixo Y local do objeto
                    float dotProduct = glm::dot(deltaWorld, yAxisDir);
                    objTransform.position = m_dragStartObjectWorldPos + yAxisDir * dotProduct;
                    break;
                }
                case GizmoHandle::None:
                default:
                    break;
                }
            }
        }


        void Gizmo2D::Render(Renderer2D& renderer, const Camera2D& camera) {
            if (!m_selectedObject) {
                return;
            }

            Transform2D& objTransform = m_selectedObject->GetTransform();

            // O tamanho do gizmo na tela deve ser independente do zoom da câmera,
            // então calculamos os tamanhos em mundo com base nos tamanhos em tela.
            float axisLengthWorld = ScreenSizeToWorldSize(TRANSLATE_AXIS_LENGTH_SCREEN_PIXELS, camera);
            float axisThicknessWorld = ScreenSizeToWorldSize(TRANSLATE_AXIS_THICKNESS_SCREEN_PIXELS, camera);
            float arrowSizeWorld = ScreenSizeToWorldSize(TRANSLATE_ARROW_SIZE_SCREEN_PIXELS, camera);

            // Calcule as posições dos eixos em coordenadas de mundo, considerando a rotação do objeto
            // Usando rotation.z que é um float (radianos)
            glm::vec2 xAxisDir = glm::rotate(glm::vec2(1.0f, 0.0f), objTransform.rotation.z);
            glm::vec2 yAxisDir = glm::rotate(glm::vec2(0.0f, 1.0f), objTransform.rotation.z);

            glm::vec2 xAxisEndWorld = objTransform.position + xAxisDir * axisLengthWorld;
            glm::vec2 yAxisEndWorld = objTransform.position + yAxisDir * axisLengthWorld;

            // --- Desenhar Handles de Translação (Eixos) ---
            // Eixo X (Vermelho)
            glm::vec4 xAxisColor = (m_activeHandle == GizmoHandle::TranslateX) ? COLOR_ACTIVE : COLOR_TRANSLATE_X;
            renderer.DrawLine(objTransform.position, xAxisEndWorld, xAxisColor, axisThicknessWorld);
            // Ponta da seta X (desenhada como um quadrado rotacionado para simplificar)
            renderer.DrawFilledRect(xAxisEndWorld, glm::vec2(arrowSizeWorld, arrowSizeWorld), xAxisColor, objTransform.rotation.z + glm::radians(45.0f));

            // Eixo Y (Verde)
            glm::vec4 yAxisColor = (m_activeHandle == GizmoHandle::TranslateY) ? COLOR_ACTIVE : COLOR_TRANSLATE_Y;
            renderer.DrawLine(objTransform.position, yAxisEndWorld, yAxisColor, axisThicknessWorld);
            // Ponta da seta Y
            renderer.DrawFilledRect(yAxisEndWorld, glm::vec2(arrowSizeWorld, arrowSizeWorld), yAxisColor, objTransform.rotation.z + glm::radians(45.0f));

            // TODO: Adicionar desenho para Handles de Rotação e Escala, se implementados
        }

    } // namespace Editor
} // namespace Luna