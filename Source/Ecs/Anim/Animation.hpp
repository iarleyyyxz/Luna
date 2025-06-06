#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <vector>
#include <memory> // Para std::shared_ptr
#include <string> // Para std::string

#include "Source/Ecs/Component.hpp" // A classe base Component
#include "Source/Renderer/Sprite.hpp" // Inclua Sprite.hpp para a definição de Sprite

// Forward declaration para SceneObject (se não incluído em Component.hpp)
class SceneObject;

namespace Luna {
    namespace Ecs {
        namespace Anim {

            // A classe Animation é um componente que gerencia animações de flipbook (quadro a quadro)
            class Animation : public Component {
            public:
                // Construtor: recebe um vetor de ponteiros para Sprites (os quadros da animação)
                // e a duração de cada quadro em segundos.
                Animation(std::shared_ptr<SceneObject> owner, const std::vector<Sprite*>& frames, float frameDuration);

                // Destrutor padrão (Sprite* não são gerenciados por esta classe).
                ~Animation() override = default;

                // Atualiza a animação com base no tempo decorrido.
                // deltaTime: tempo em segundos desde a última atualização.
                void Update(float deltaTime) override;

                // Implementação do método OnGui para ImGui.
                void OnGui() override;

                // Reinicia a animação para o primeiro quadro.
                void Reset();

                // Obtém o Sprite do quadro de animação atual.
                Sprite* GetCurrentFrame() const;

                // Define se a animação deve ser repetida (loop) ou parar no último quadro.
                void SetLooping(bool looping) { m_looping = looping; }

                // Verifica se a animação terminou (apenas se não estiver em loop).
                bool IsFinished() const { return !m_looping && m_isFinished; }

            private:
                std::vector<Sprite*> m_frames;      // Ponteiros para os sprites que compõem a animação
                float m_frameDuration;              // Duração de cada quadro em segundos
                float m_currentTime;                // Tempo acumulado para o quadro atual
                int m_currentFrameIndex;            // Índice do quadro atual
                bool m_looping;                     // Indica se a animação deve ser repetida
                bool m_isFinished;                  // Indica se a animação terminou (para não-looping)
            };

        } // namespace Anim
    } // namespace Ecs
} // namespace Luna

#endif // ANIMATION_HPP