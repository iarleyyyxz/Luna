#include "Animation.hpp"
#include <iostream>

Animation::Animation(const std::vector<Sprite*>& frames, float frameDuration)
    : m_frames(frames), m_frameDuration(frameDuration),
    m_currentTime(0.0f), m_currentFrameIndex(0),
    m_looping(true), m_isFinished(false)
{
    if (m_frames.empty()) {
        std::cerr << "AVISO::ANIMATION::CONSTRUTOR: Animação criada sem frames." << std::endl;
    }
}

Animation::~Animation() {
    // In this model, the animation is NOT responsible for deleting the Sprite pointers
// it contains. The Spritesheet is the "owner" of these Sprites.
// If the animation were the owner, we would need to delete each Sprite here.

}

void Animation::Update(float deltaTime) {
    {
        if (m_frames.empty() || m_isFinished) {
            return;
        }

        m_currentTime += deltaTime;

        // Advances to the next frame if the current frame's time exceeds the duration
        if (m_currentTime >= m_frameDuration) {
            m_currentTime -= m_frameDuration; // Subtracts the frame duration to keep the remaining time
            m_currentFrameIndex++;

            // Verifica se chegamos ao fim da animação
            if (m_currentFrameIndex >= m_frames.size()) {
                if (m_looping) {
                    m_currentFrameIndex = 0; // Volta para o início se estiver em loop
                }
                else {
                    m_currentFrameIndex = m_frames.size() - 1; // Fica no último frame
                    m_isFinished = true; // Marca a animação como terminada
                }
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