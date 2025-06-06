#include "Animation.hpp"
#include "Source/Ecs/SceneObject.hpp"   // Para SceneObject
#include "Source/Ecs/Transform2D.hpp"   // Para Transform2D
#include "Source/Renderer/Sprite.hpp"   // Para Sprite (se você precisar de acesso direto a membros)
#include <iostream>                     // Para std::cerr
#include <algorithm>                    // Para std::sort (se você reintroduzir keyframes de interpolação)
#include <imgui.h>                      // Para ImGui

namespace Luna {
    namespace Ecs {
        namespace Anim {

            // Construtor da Animação
            Animation::Animation(std::shared_ptr<SceneObject> owner, const std::vector<Sprite*>& frames, float frameDuration)
                : Component(), // Chame o construtor da base com um nome
                m_frames(frames), m_frameDuration(frameDuration),
                m_currentTime(0.0f), m_currentFrameIndex(0),
                m_looping(true), m_isFinished(false)
            {
                // O owner será setado por SceneObject::AddComponent.
                // Se o owner já estiver sendo passado no construtor da base, não é necessário aqui.
                // Component("Animation", owner)
                SetOwner(owner); // Garanta que o owner seja definido

                if (m_frames.empty()) {
                    std::cerr << "AVISO::ANIMATION::CONSTRUTOR: Animação criada sem frames." << std::endl;
                }
            }

            // O Destrutor padrão é suficiente, pois a Animação não é proprietária dos Sprites.

            void Animation::Update(float deltaTime) {
                // Obtenha o SceneObject proprietário
                std::shared_ptr<SceneObject> owner = GetOwner();
                if (!owner) {
                    // Isso pode acontecer se o owner foi destruído antes do componente.
                    return;
                }

                // Obtenha o Transform2D do SceneObject proprietário
                Transform2D& transform = owner->GetTransform(); // Acesse o Transform2D diretamente

                // A lógica de animação de flipbook (quadro a quadro)
                if (m_frames.empty() || m_isFinished) {
                    return;
                }

                m_currentTime += deltaTime;

                if (m_currentTime >= m_frameDuration) {
                    m_currentTime -= m_frameDuration; // Subtrai a duração do quadro para manter o tempo restante
                    m_currentFrameIndex++;

                    if (m_currentFrameIndex >= m_frames.size()) {
                        if (m_looping) {
                            m_currentFrameIndex = 0; // Volta para o início se estiver em loop
                        }
                        else {
                            m_currentFrameIndex = m_frames.size() - 1; // Fica no último quadro
                            m_isFinished = true; // Marca a animação como terminada
                        }
                    }
                }

                // Opcional: Se a animação precisar de ajustar a posição, rotação ou escala do owner
                // com base no Sprite atual, faça-o aqui.
                // Exemplo: Se o Sprite tem um offset ou um tamanho diferente
                // Sprite* currentSprite = GetCurrentFrame();
                // if (currentSprite) {
                //     // Ajuste a escala do transform para o tamanho do sprite, por exemplo.
                //     // transform.scale.x = static_cast<float>(currentSprite->GetWidth());
                //     // transform.scale.y = static_cast<float>(currentSprite->GetHeight());
                // }
            }

            void Animation::OnGui() {
                if (ImGui::TreeNodeEx("Animation", ImGuiTreeNodeFlags_DefaultOpen)) {
                    ImGui::Checkbox("Looping", &m_looping);
                    ImGui::DragFloat("Frame Duration", &m_frameDuration, 0.01f, 0.01f, 10.0f);
                    ImGui::Text("Current Time: %.2f", m_currentTime);
                    ImGui::Text("Current Frame: %d / %zu", m_currentFrameIndex, m_frames.size());
                    ImGui::Checkbox("Finished", &m_isFinished);

                    if (ImGui::Button("Reset")) {
                        Reset();
                    }
                    ImGui::TreePop();
                }
            }


            void Animation::Reset() {
                m_currentFrameIndex = 0;
                m_currentTime = 0.0f;
                m_isFinished = false;
            }

            Sprite* Animation::GetCurrentFrame() const {
                if (m_frames.empty()) {
                    return nullptr;
                }
                return m_frames[m_currentFrameIndex];
            }

        } // namespace Anim
    } // namespace Ecs
} // namespace Luna
