#ifndef CAMERA2D_HPP
#define CAMERA2D_HPP

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

class Camera2D {
public:
    Camera2D(float left, float right, float bottom, float top);
    glm::mat4 getProjectionMatrix() const;

private:
    glm::mat4 m_projectionMatrix;
};

#endif // CAMERA2D_HPP