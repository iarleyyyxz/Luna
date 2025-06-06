#include "Animation.hpp"
#include "Source/Ecs/SceneObject.hpp"   // Para SceneObject
#include "Source/Ecs/Transform2D.hpp"   // Para Transform2D
#include "Source/Renderer/Sprite.hpp"   // Para Sprite (se voc� precisar de acesso direto a membros)
#include <iostream>                     // Para std::cerr
#include <algorithm>                    // Para std::sort (se voc� reintroduzir keyframes de interpola��o)
#include <imgui.h>                      // Para ImGui

namespace Luna {
    namespace Ecs {
        namespace Anim {

            // Construtor da Anima��o
            Animation::Animation(std::shared_ptr<SceneObject> owner, const std::vector<Sprite*>& frames, float frameDuration)
                : Component(), // Chame o construtor da base com um nome
                m_frames(frames), m_frameDuration(frameDuration),
                m_currentTime(0.0f), m_currentFrameIndex(0),
                m_looping(true), m_isFinished(false)
            {
                // O owner ser� setado por SceneObject::AddComponent.
                // Se o owner j� estiver sendo passado no construtor da base, n�o � necess�rio aqui.
                // Component("Animation", owner)
                SetOwner(owner); // Garanta que o owner seja definido

                if (m_frames.empty()) {
                    std::cerr << "AVISO::ANIMATION::CONSTRUTOR: Anima��o criada sem frames." << std::endl;
                }
            }

            // O Destrutor padr�o � suficiente, pois a Anima��o n�o � propriet�ria dos Sprites.

            void Animation::Update(float deltaTime) {
                // Obtenha o SceneObject propriet�rio
                std::shared_ptr<SceneObject> owner = GetOwner();
                if (!owner) {
                    // Isso pode acontecer se o owner foi destru�do antes do componente.
                    return;
                }

                // Obtenha o Transform2D do SceneObject propriet�rio
                Transform2D& transform = owner->GetTransform(); // Acesse o Transform2D diretamente

                // A l�gica de anima��o de flipbook (quadro a quadro)
                if (m_frames.empty() || m_isFinished) {
                    return;
                }

                m_currentTime += deltaTime;

                if (m_currentTime >= m_frameDuration) {
                    m_currentTime -= m_frameDuration; // Subtrai a dura��o do quadro para manter o tempo restante
                    m_currentFrameIndex++;

                    if (m_currentFrameIndex >= m_frames.size()) {
                        if (m_looping) {
                            m_currentFrameIndex = 0; // Volta para o in�cio se estiver em loop
                        }
                        else {
                            m_currentFrameIndex = m_frames.size() - 1; // Fica no �ltimo quadro
                            m_isFinished = true; // Marca a anima��o como terminada
                        }
                    }
                }

                // Opcional: Se a anima��o precisar de ajustar a posi��o, rota��o ou escala do owner
                // com base no Sprite atual, fa�a-o aqui.
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
