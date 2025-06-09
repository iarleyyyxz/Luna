#pragma once

#include <GLFW/glfw3.h> // Para defini��es de bot�es GLFW_MOUSE_BUTTON_*
#include <glm.hpp>   // Para glm::vec2
#include <iostream>
#include <unordered_map> // Para std::unordered_map

namespace Luna { // Assumindo que sua classe Mouse est� dentro de Luna::Input
    namespace Input {

        class Mouse {
        public:
            // Construtor: Opcional, mas �til para inicializar m_buttonStates se necess�rio
            Mouse() = default;

            void ProcessCursorPosition(double x, double y);
            void ProcessMouseButton(int button, int action);
            void UpdatePosition(GLFWwindow* window); // Atualiza a posi��o do mouse a cada frame

            void ProcessScroll(double xoffset, double yoffset);
            float GetScrollOffsetY() const;
            float GetScrollOffsetX() const;
            void ClearScroll(); // Chame no fim de cada frame se o scroll for cumulativo

            glm::vec2 GetPosition() const { return m_position; }

            // *** NOVO: M�todo para verificar se um bot�o do mouse est� pressionado ***
            bool IsButtonPressed(int button) const;

        private:
            glm::vec2 m_position;
            std::unordered_map<int, bool> m_buttonStates; // true se pressionado, false se solto

            float scrollOffsetX = 0.0f;
            float scrollOffsetY = 0.0f;
        };

    } // namespace Input
} // namespace Luna