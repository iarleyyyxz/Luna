#ifndef CAMERA2D_HPP
#define CAMERA2D_HPP

#include <glm.hpp> // Para glm::vec2, glm::mat4
#include <matrix_transform.hpp> // Para glm::translate, glm::rotate, glm::scale

class Camera2D {
public:
    Camera2D();
    ~Camera2D();

    // Define a posi��o da c�mara no mundo 2D
    void setPosition(const glm::vec2& position);
    // Obt�m a posi��o atual da c�mara
    glm::vec2 getPosition() const { return m_position; }

    // Define o n�vel de zoom da c�mara (1.0f � zoom normal)
    void setZoom(float zoom);
    // Obt�m o n�vel de zoom atual
    float getZoom() const { return m_zoom; }

    // Define a rota��o da c�mara em graus
    void setRotation(float rotationDegrees);
    // Obt�m a rota��o atual em graus
    float getRotation() const { return m_rotationDegrees; }

    // Atualiza a matriz de visualiza��o da c�mara
    // Esta matriz � usada para "mover" o mundo em rela��o � c�mara
    glm::mat4 getViewMatrix() const;

private:
    glm::vec2 m_position;        // Posi��o da c�mera no mundo (centro da vista)
    float m_zoom;                // N�vel de zoom (1.0f = 100%)
    float m_rotationDegrees;     // Rota��o em graus
};

#endif // CAMERA2D_HPP
