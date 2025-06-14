#pragma once

#include <GLFW/glfw3.h>          // Para definições de botões GLFW_MOUSE_BUTTON_*
#include <glm.hpp>           // Para glm::vec2
#include <iostream>
#include <unordered_map>         // Para std::unordered_map

namespace Luna {
    namespace Input {

        class Mouse {
        public:
            Mouse() = default;

            void ProcessCursorPosition(double x, double y);
            // *** CORRIGIDO: Adicionado 'int mods' ***
            void ProcessMouseButton(int button, int action, int mods);
            void UpdatePosition(GLFWwindow* window);

            void ProcessScroll(double xoffset, double yoffset);
            float GetScrollOffsetY() const;
            float GetScrollOffsetX() const;
            void ClearScroll();

            glm::vec2 GetPosition() const { return m_position; }

            bool IsButtonPressed(int button) const;

        private:
            glm::vec2 m_position;
            std::unordered_map<int, bool> m_buttonStates;

            float scrollOffsetX = 0.0f;
            float scrollOffsetY = 0.0f;
        };

    } // namespace Input
} // namespace Luna
