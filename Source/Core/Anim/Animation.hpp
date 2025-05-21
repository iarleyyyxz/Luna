#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <vector>
#include "Sprite.hpp" // A animação gerencia ponteiros para Sprites

class Animation {
public:
    // Construtor: recebe um vetor de ponteiros para Sprites (os frames da animação)
    // e a duração de cada frame em segundos.
    Animation(const std::vector<Sprite*>& frames, float frameDuration);

    // Destrutor: libera a memória dos Sprites que a animação é responsável por gerenciar.
    // Se os Sprites forem gerenciados pela Spritesheet, a animação não os deleta.
    // Neste caso, a Spritesheet é dona dos Sprites, então a animação apenas os referencia.
    ~Animation();

    // Atualiza a animação com base no tempo decorrido.
    // deltaTime: tempo em segundos desde a última atualização.
    void Update(float deltaTime);

    // Reinicia a animação para o primeiro frame.
    void Reset();

    // Obtém o Sprite do frame atual da animação.
    Sprite* GetCurrentFrame() const;

    // Define se a animação deve repetir (loop) ou parar no último frame.
    void SetLooping(bool looping) { m_looping = looping; }

    // Verifica se a animação terminou (apenas se não estiver em loop).
    bool IsFinished() const { return !m_looping && m_isFinished; }

private:
    std::vector<Sprite*> m_frames; // Ponteiros para os sprites que compõem a animação
    float m_frameDuration;         // Duração de cada frame em segundos
    float m_currentTime;           // Tempo acumulado para o frame atual
    int m_currentFrameIndex;       // Índice do frame atual
    bool m_looping;                // Se a animação deve repetir
    bool m_isFinished;             // Se a animação terminou (para não looping)
};

#endif // ANIMATION_HPP
