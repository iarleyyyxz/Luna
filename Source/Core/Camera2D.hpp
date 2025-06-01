#ifndef CAMERA2D_HPP
#define CAMERA2D_HPP

#include <glm.hpp>
#include <memory>

#include "Efx/ParallaxLayerData.hpp"

#include "Source/Ecs/Transform2D.hpp"

#include "Source/Renderer/Renderer2D.hpp"

class Camera2D {
public:
    Camera2D(float viewportWidth, float viewportHeight);
    ~Camera2D() = default;

    void setViewportSize(float width, float height);
    glm::vec2 getPosition() const { return m_position; }
    void setPosition(const glm::vec2& position);

    void setTarget(std::weak_ptr<Transform2D> target);
    void setFollowSmoothness(float smoothness);

    float getZoom() const;
    void setZoom(float zoom);

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;

    void update(float deltaTime);

private:
    glm::vec2 m_position = glm::vec2(0.0f);
    float m_viewportWidth;
    float m_viewportHeight;
    std::weak_ptr<Transform2D> m_target;
    float m_followSmoothness = 0.1f;
    float m_zoom = 1.0f; // Zoom padrão
};

#endif // CAMERA2D_HPP