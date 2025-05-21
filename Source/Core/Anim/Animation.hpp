#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <vector>
#include "Sprite.hpp" // A anima��o gerencia ponteiros para Sprites

class Animation {
public:
    // Construtor: recebe um vetor de ponteiros para Sprites (os frames da anima��o)
    // e a dura��o de cada frame em segundos.
    Animation(const std::vector<Sprite*>& frames, float frameDuration);

    // Destrutor: libera a mem�ria dos Sprites que a anima��o � respons�vel por gerenciar.
    // Se os Sprites forem gerenciados pela Spritesheet, a anima��o n�o os deleta.
    // Neste caso, a Spritesheet � dona dos Sprites, ent�o a anima��o apenas os referencia.
    ~Animation();

    // Atualiza a anima��o com base no tempo decorrido.
    // deltaTime: tempo em segundos desde a �ltima atualiza��o.
    void Update(float deltaTime);

    // Reinicia a anima��o para o primeiro frame.
    void Reset();

    // Obt�m o Sprite do frame atual da anima��o.
    Sprite* GetCurrentFrame() const;

    // Define se a anima��o deve repetir (loop) ou parar no �ltimo frame.
    void SetLooping(bool looping) { m_looping = looping; }

    // Verifica se a anima��o terminou (apenas se n�o estiver em loop).
    bool IsFinished() const { return !m_looping && m_isFinished; }

private:
    std::vector<Sprite*> m_frames; // Ponteiros para os sprites que comp�em a anima��o
    float m_frameDuration;         // Dura��o de cada frame em segundos
    float m_currentTime;           // Tempo acumulado para o frame atual
    int m_currentFrameIndex;       // �ndice do frame atual
    bool m_looping;                // Se a anima��o deve repetir
    bool m_isFinished;             // Se a anima��o terminou (para n�o looping)
};

#endif // ANIMATION_HPP
