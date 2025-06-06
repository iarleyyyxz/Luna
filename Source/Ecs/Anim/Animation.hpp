#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <vector>
#include <memory> // Para std::shared_ptr
#include <string> // Para std::string

#include "Source/Ecs/Component.hpp" // A classe base Component
#include "Source/Renderer/Sprite.hpp" // Inclua Sprite.hpp para a defini��o de Sprite

// Forward declaration para SceneObject (se n�o inclu�do em Component.hpp)
class SceneObject;

namespace Luna {
    namespace Ecs {
        namespace Anim {

            // A classe Animation � um componente que gerencia anima��es de flipbook (quadro a quadro)
            class Animation : public Component {
            public:
                // Construtor: recebe um vetor de ponteiros para Sprites (os quadros da anima��o)
                // e a dura��o de cada quadro em segundos.
                Animation(std::shared_ptr<SceneObject> owner, const std::vector<Sprite*>& frames, float frameDuration);

                // Destrutor padr�o (Sprite* n�o s�o gerenciados por esta classe).
                ~Animation() override = default;

                // Atualiza a anima��o com base no tempo decorrido.
                // deltaTime: tempo em segundos desde a �ltima atualiza��o.
                void Update(float deltaTime) override;

                // Implementa��o do m�todo OnGui para ImGui.
                void OnGui() override;

                // Reinicia a anima��o para o primeiro quadro.
                void Reset();

                // Obt�m o Sprite do quadro de anima��o atual.
                Sprite* GetCurrentFrame() const;

                // Define se a anima��o deve ser repetida (loop) ou parar no �ltimo quadro.
                void SetLooping(bool looping) { m_looping = looping; }

                // Verifica se a anima��o terminou (apenas se n�o estiver em loop).
                bool IsFinished() const { return !m_looping && m_isFinished; }

            private:
                std::vector<Sprite*> m_frames;      // Ponteiros para os sprites que comp�em a anima��o
                float m_frameDuration;              // Dura��o de cada quadro em segundos
                float m_currentTime;                // Tempo acumulado para o quadro atual
                int m_currentFrameIndex;            // �ndice do quadro atual
                bool m_looping;                     // Indica se a anima��o deve ser repetida
                bool m_isFinished;                  // Indica se a anima��o terminou (para n�o-looping)
            };

        } // namespace Anim
    } // namespace Ecs
} // namespace Luna

#endif // ANIMATION_HPP