#include "Mouse.hpp"
#include <GLFW/glfw3.h>

namespace Luna {
    namespace Input {

        void Mouse::ProcessCursorPosition(double x, double y) {
            m_position = glm::vec2(x, y);
        }

        // *** CORRIGIDO: Adicionado 'int mods' ***
        void Mouse::ProcessMouseButton(int button, int action, int mods) {
            m_buttonStates[button] = (action == GLFW_PRESS);
            // O 'mods' pode ser usado aqui se você precisar saber se Shift/Ctrl/Alt estavam pressionados
            // Ex: if (mods & GLFW_MOD_SHIFT) { /* Fazer algo com Shift */ }
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

        bool Mouse::IsButtonPressed(int button) const {
            auto it = m_buttonStates.find(button);
            return it != m_buttonStates.end() && it->second;
        }

    } // namespace Input
} // namespace Luna
