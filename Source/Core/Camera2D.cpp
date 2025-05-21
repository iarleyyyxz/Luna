#include "Camera2D.hpp"
#include <matrix_transform.hpp> // Para glm::translate, glm::rotate, glm::scale
#include <constants.hpp>        // Para glm::pi (se for usar radianos)

Camera2D::Camera2D() : m_position(0.0f, 0.0f), m_zoom(1.0f), m_rotationDegrees(0.0f) {
    // Construtor: Inicializa a c�mara na origem, sem zoom e sem rota��o
}

Camera2D::~Camera2D() {
    // Destrutor: N�o h� recursos din�micos para liberar aqui
}

void Camera2D::setPosition(const glm::vec2& position) {
    m_position = position;
}

void Camera2D::setZoom(float zoom) {
    // Garante que o zoom n�o seja zero ou negativo para evitar divis�o por zero
    m_zoom = glm::max(0.001f, zoom);
}

void Camera2D::setRotation(float rotationDegrees) {
    m_rotationDegrees = rotationDegrees;
}

glm::mat4 Camera2D::getViewMatrix() const {
    glm::mat4 view = glm::mat4(1.0f); // Matriz identidade inicial

    // A ordem de aplica��o das transforma��es inversas � crucial para uma matriz de visualiza��o.
    // A matriz de visualiza��o � o inverso da transforma��o do mundo da c�mara.
    // Ordem: Inverse Scale -> Inverse Rotate -> Inverse Translate

    // 1. Zoom (Inverse Scale): Escalar o mundo pelo inverso do zoom da c�mara
    view = glm::scale(view, glm::vec3(1.0f / m_zoom, 1.0f / m_zoom, 1.0f));

    // 2. Rota��o (Inverse Rotate): Rodar o mundo na dire��o oposta � rota��o da c�mara
    // Note o sinal negativo na rota��o para inverter a dire��o
    view = glm::rotate(view, glm::radians(-m_rotationDegrees), glm::vec3(0.0f, 0.0f, 1.0f));

    // 3. Transla��o (Inverse Translate): Mover o mundo pelo inverso da posi��o da c�mara
    // A posi��o da c�mara � o centro da vista, ent�o movemos o mundo pelo negativo dessa posi��o
    view = glm::translate(view, glm::vec3(-m_position.x, -m_position.y, 0.0f));

    return view;
}
