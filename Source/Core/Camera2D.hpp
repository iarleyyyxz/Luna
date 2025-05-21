#ifndef CAMERA2D_HPP
#define CAMERA2D_HPP

#include <glm.hpp> // Para glm::vec2, glm::mat4
#include <matrix_transform.hpp> // Para glm::translate, glm::rotate, glm::scale

class Camera2D {
public:
    Camera2D();
    ~Camera2D();

    // Define a posição da câmara no mundo 2D
    void setPosition(const glm::vec2& position);
    // Obtém a posição atual da câmara
    glm::vec2 getPosition() const { return m_position; }

    // Define o nível de zoom da câmara (1.0f é zoom normal)
    void setZoom(float zoom);
    // Obtém o nível de zoom atual
    float getZoom() const { return m_zoom; }

    // Define a rotação da câmara em graus
    void setRotation(float rotationDegrees);
    // Obtém a rotação atual em graus
    float getRotation() const { return m_rotationDegrees; }

    // Atualiza a matriz de visualização da câmara
    // Esta matriz é usada para "mover" o mundo em relação à câmara
    glm::mat4 getViewMatrix() const;

private:
    glm::vec2 m_position;        // Posição da câmera no mundo (centro da vista)
    float m_zoom;                // Nível de zoom (1.0f = 100%)
    float m_rotationDegrees;     // Rotação em graus
};

#endif // CAMERA2D_HPP
