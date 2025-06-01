#include "Camera2D.hpp"
#include <matrix_transform.hpp>
#include <iostream>
#include <algorithm> // Para glm::clamp
 // Uma forma simples de acessar o renderer (melhor usar injeção de dependência)

Camera2D::Camera2D(float viewportWidth, float viewportHeight) :
    m_viewportWidth(viewportWidth), m_viewportHeight(viewportHeight) {
    std::cout << "Câmera 2D criada com viewport " << viewportWidth << "x" << viewportHeight << std::endl;
}

void Camera2D::setViewportSize(float width, float height) {
    m_viewportWidth = width;
    m_viewportHeight = height;
}

void Camera2D::setPosition(const glm::vec2& position) {
    m_position = position;
}

void Camera2D::setTarget(std::weak_ptr<Transform2D> target) {
    m_target = target;
}

void Camera2D::setFollowSmoothness(float smoothness) {
    m_followSmoothness = glm::clamp(smoothness, 0.0f, 1.0f);
}

float Camera2D::getZoom() const {
    return m_zoom;
}

void Camera2D::setZoom(float zoom) {
    m_zoom = glm::max(0.1f, zoom); // Impede zoom muito pequeno
}

glm::mat4 Camera2D::getViewMatrix() const {
    return glm::translate(glm::mat4(1.0f), glm::vec3(-m_position, 0.0f));
}

glm::mat4 Camera2D::getProjectionMatrix() const {
    float halfWidth = m_viewportWidth / (2.0f * m_zoom);
    float halfHeight = m_viewportHeight / (2.0f * m_zoom);
    return glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, -1.0f, 1.0f);
}

void Camera2D::update(float deltaTime) {
    if (auto targetPtr = m_target.lock()) {
        glm::vec2 targetPosition = targetPtr->position;
        m_position = glm::mix(m_position, targetPosition, m_followSmoothness);
    }
}