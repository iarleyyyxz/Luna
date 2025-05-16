#include "camera.hpp"

Camera2D::Camera2D(float left, float right, float bottom, float top) {
	this->m_projectionMatrix = glm::ortho(left, right, bottom, top);
}

glm::mat4 Camera2D::getProjectionMatrix() const {
	return this->m_projectionMatrix;
}