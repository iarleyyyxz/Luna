#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <vector>
#include "Sprite.hpp" // The animation manages pointers to Sprites

class Animation {
public:
    // Constructor: receives a vector of pointers to Sprites (the animation frames)
    // and the duration of each frame in seconds.
    Animation(const std::vector<Sprite*>& frames, float frameDuration);

    // Destructor: frees the memory of the Sprites that the animation is responsible for managing.
    // If the Sprites are managed by the Spritesheet, the animation does not delete them.
    // In that case, the Spritesheet owns the Sprites, so the animation only references them.
    ~Animation();

    // Updates the animation based on the elapsed time.
    // deltaTime: time in seconds since the last update.
    void Update(float deltaTime);

    // Resets the animation to the first frame.
    void Reset();

    // Gets the Sprite of the current animation frame.
    Sprite* GetCurrentFrame() const;

    // Sets whether the animation should loop or stop at the last frame.
    void SetLooping(bool looping) { m_looping = looping; }

    // Checks whether the animation has finished (only if not looping).
    bool IsFinished() const { return !m_looping && m_isFinished; }

private:
    std::vector<Sprite*> m_frames; // Pointers to the sprites that make up the animation
    float m_frameDuration;         // Duration of each frame in seconds
    float m_currentTime;           // Accumulated time for the current frame
    int m_currentFrameIndex;       // Index of the current frame
    bool m_looping;                // Whether the animation should loop
    bool m_isFinished;             // Whether the animation has finished (for non-looping)
};

#endif // ANIMATION_HPP
