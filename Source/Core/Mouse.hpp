// Mouse.hpp
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <iostream>
#include <unordered_map>

class Mouse {
public:
    void ProcessCursorPosition(double x, double y);
    void ProcessMouseButton(int button, int action);
    void UpdatePosition(GLFWwindow* window);

    void ProcessScroll(double xoffset, double yoffset);
    float GetScrollOffsetY() const;
    float GetScrollOffsetX() const;
    void ClearScroll(); // Chame no fim de cada frame

    glm::vec2 GetPosition() const { return m_position; }

private:
    glm::vec2 m_position;
    std::unordered_map<int, bool> m_buttonStates;

    float scrollOffsetX = 0.0f;
    float scrollOffsetY = 0.0f;
};
