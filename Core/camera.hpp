#ifndef CAMERA2D_HPP
#define CAMERA2D_HPP

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

class Camera2D {

public:
    Camera2D(float left, float right, float bottom, float top);

    void setPosition(const glm::vec2& position);
    const glm::vec2& getPosition() const { return m_position; }

    void setZoom(float zoom);
    float getZoom() const { return m_zoom; }

    void move(const glm::vec2& offset);

    void setViewportSize(float width, float height);

    void follow(const glm::vec2& targetPosition);

    glm::mat4 getProjectionMatrix() const;

private:
    void updateProjectionMatrix();

    glm::mat4 m_projectionMatrix;
    glm::vec2 m_position;
    float m_zoom;
    float m_viewportWidth;
    float m_viewportHeight;
};

#endif // CAMERA2D_HPP