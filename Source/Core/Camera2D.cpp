#include "Camera2D.hpp"
#include <matrix_transform.hpp> // Para glm::translate, glm::rotate, glm::scale
#include <constants.hpp>        // Para glm::pi (se for usar radianos)

Camera2D::Camera2D() : m_position(0.0f, 0.0f), m_zoom(1.0f), m_rotationDegrees(0.0f) {
    // Construtor: Inicializa a câmara na origem, sem zoom e sem rotação
}

Camera2D::~Camera2D() {
    // Destrutor: Não há recursos dinâmicos para liberar aqui
}

void Camera2D::setPosition(const glm::vec2& position) {
    m_position = position;
}

void Camera2D::setZoom(float zoom) {
    // Garante que o zoom não seja zero ou negativo para evitar divisão por zero
    m_zoom = glm::max(0.001f, zoom);
}

void Camera2D::setRotation(float rotationDegrees) {
    m_rotationDegrees = rotationDegrees;
}

glm::mat4 Camera2D::getViewMatrix() const {
    glm::mat4 view = glm::mat4(1.0f); // Matriz identidade inicial

    // A ordem de aplicação das transformações inversas é crucial para uma matriz de visualização.
    // A matriz de visualização é o inverso da transformação do mundo da câmara.
    // Ordem: Inverse Scale -> Inverse Rotate -> Inverse Translate

    // 1. Zoom (Inverse Scale): Escalar o mundo pelo inverso do zoom da câmara
    view = glm::scale(view, glm::vec3(1.0f / m_zoom, 1.0f / m_zoom, 1.0f));

    // 2. Rotação (Inverse Rotate): Rodar o mundo na direção oposta à rotação da câmara
    // Note o sinal negativo na rotação para inverter a direção
    view = glm::rotate(view, glm::radians(-m_rotationDegrees), glm::vec3(0.0f, 0.0f, 1.0f));

    // 3. Translação (Inverse Translate): Mover o mundo pelo inverso da posição da câmara
    // A posição da câmara é o centro da vista, então movemos o mundo pelo negativo dessa posição
    view = glm::translate(view, glm::vec3(-m_position.x, -m_position.y, 0.0f));

    return view;
}
