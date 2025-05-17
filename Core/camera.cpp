#include "camera.hpp"

Camera2D::Camera2D(float left, float right, float bottom, float top)
    : m_position(glm::vec2(0.0f)), m_zoom(1.0f), m_viewportWidth(right - left), m_viewportHeight(top - bottom)
{
    updateProjectionMatrix();
}

void Camera2D::setPosition(const glm::vec2& position) {
    m_position = position;
    updateProjectionMatrix();
}

void Camera2D::setZoom(float zoom) {
    m_zoom = zoom;
    updateProjectionMatrix();
}

void Camera2D::move(const glm::vec2& offset) {
    m_position += offset;
    updateProjectionMatrix();
}

void Camera2D::setViewportSize(float width, float height) {
    m_viewportWidth = width;
    m_viewportHeight = height;
    updateProjectionMatrix();
}

void Camera2D::follow(const glm::vec2& targetPosition) {
    m_position.x = targetPosition.x - m_viewportWidth / (2.0f * m_zoom);
    m_position.y = targetPosition.y - m_viewportHeight / (2.0f * m_zoom);
    updateProjectionMatrix();
}

void Camera2D::updateProjectionMatrix() {
    float halfWidth = m_viewportWidth / (2.0f * m_zoom);
    float halfHeight = m_viewportHeight / (2.0f * m_zoom);
    m_projectionMatrix = glm::ortho(m_position.x - halfWidth, m_position.x + halfWidth,
        m_position.y - halfHeight, m_position.y + halfHeight);
}

glm::mat4 Camera2D::getProjectionMatrix() const {
    return m_projectionMatrix;
}