#include "Mouse.hpp"
#include <GLFW/glfw3.h> // Certifique-se de que está incluído para definições GLFW_MOUSE_BUTTON_*

namespace Luna {
    namespace Input {

        void Mouse::ProcessCursorPosition(double x, double y) {
            m_position = glm::vec2(x, y);
        }

        void Mouse::ProcessMouseButton(int button, int action) {
            // Armazena o estado do botão
            // true se a ação for GLFW_PRESS, false se for GLFW_RELEASE
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

        // *** NOVO: Implementação do método IsButtonPressed ***
        bool Mouse::IsButtonPressed(int button) const {
            // Procura o botão no mapa. Se encontrado e o valor for true, está pressionado.
            // Se não encontrado, assume false (não pressionado/estado desconhecido).
            auto it = m_buttonStates.find(button);
            return it != m_buttonStates.end() && it->second;
        }

    } // namespace Input
} // namespace Luna