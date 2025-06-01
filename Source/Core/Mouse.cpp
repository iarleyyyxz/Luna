// Mouse.cpp
#include "Mouse.hpp"

void Mouse::ProcessCursorPosition(double x, double y) {
    m_position = glm::vec2(x, y);
}

void Mouse::ProcessMouseButton(int button, int action) {
    m_buttonStates[button] = (action == GLFW_PRESS);
}

void Mouse::UpdatePosition(GLFWwindow* window) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    m_position = glm::vec2(xpos, ypos);
}

void Mouse::ProcessScroll(double xoffset, double yoffset) {
    scrollOffsetX += static_cast<float>(xoffset);
    scrollOffsetY += static_cast<float>(yoffset);
}

float Mouse::GetScrollOffsetY() const {
    return scrollOffsetY;
}

float Mouse::GetScrollOffsetX() const {
    return scrollOffsetX;
}

void Mouse::ClearScroll() {
    scrollOffsetX = 0.0f;
    scrollOffsetY = 0.0f;
}
