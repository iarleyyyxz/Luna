#include "Animation.hpp"
#include <iostream>

Animation::Animation(const std::vector<Sprite*>& frames, float frameDuration)
    : m_frames(frames), m_frameDuration(frameDuration),
    m_currentTime(0.0f), m_currentFrameIndex(0),
    m_looping(true), m_isFinished(false)
{
    if (m_frames.empty()) {
        std::cerr << "AVISO::ANIMATION::CONSTRUTOR: Anima��o criada sem frames." << std::endl;
    }
}

Animation::~Animation() {
    // Neste modelo, a anima��o N�O � respons�vel por deletar os ponteiros de Sprite
    // que ela cont�m. A Spritesheet � a "dona" desses Sprites.
    // Se a anima��o fosse a dona, precisar�amos deletar cada Sprite aqui.
}

void Animation::Update(float deltaTime) {
    if (m_frames.empty() || m_isFinished) {
        return;
    }

    m_currentTime += deltaTime;

    // Avan�a para o pr�ximo frame se o tempo do frame atual exceder a dura��o
    if (m_currentTime >= m_frameDuration) {
        m_currentTime -= m_frameDuration; // Subtrai a dura��o do frame para manter o resto do tempo
        m_currentFrameIndex++;

        // Verifica se chegamos ao fim da anima��o
        if (m_currentFrameIndex >= m_frames.size()) {
            if (m_looping) {
                m_currentFrameIndex = 0; // Volta para o in�cio se estiver em loop
            }
            else {
                m_currentFrameIndex = m_frames.size() - 1; // Fica no �ltimo frame
                m_isFinished = true; // Marca a anima��o como terminada
            }
        }
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
