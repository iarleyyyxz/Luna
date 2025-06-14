#pragma once

#include <GLFW/glfw3.h> // Inclui a biblioteca GLFW para manipulação de gamepad.
#include <glm.hpp>  // Inclui GLM para tipos de vetor (como glm::vec2).
#include <string>       // Inclui para uso de std::string.
#include <iostream>     // Inclui para saída de console (std::cout).

namespace Luna {
    namespace Input {

        class Gamepad {
        public:
            // Construtor: Inicializa o gamepad com um ID específico.
            Gamepad(int id);

            // Atualiza o estado do gamepad (botões e eixos). Retorna true se o estado foi atualizado com sucesso, false caso contrário.
            bool Update();
            // Verifica se o gamepad está conectado e é reconhecido como um gamepad padrão.
            bool IsConnected() const;
            // Retorna o nome do gamepad.
            std::string GetName() const;

            // --- Métodos de Consulta de Botões Genéricos ---
            // Verifica se um botão específico está pressionado.
            bool IsButtonPressed(int button) const;
            // Verifica se um botão foi *acabado de* pressionar (estava solto e agora está pressionado).
            bool IsButtonJustPressed(int button) const;
            // Verifica se um botão foi *acabado de* soltar (estava pressionado e agora está solto).
            bool IsButtonJustReleased(int button) const;

            // --- Métodos de Consulta de Eixos Genéricos ---
            // Retorna o valor de um eixo específico (varia de -1.0 a 1.0).
            float GetAxisValue(int axis) const;

            // --- Métodos de Conveniência (Específicos para Gamepads Padrão) ---
            // Retorna o vetor (x, y) do stick esquerdo.
            glm::vec2 GetLeftStick() const;
            // Retorna o vetor (x, y) do stick direito.
            glm::vec2 GetRightStick() const;
            // Retorna o valor do gatilho esquerdo (0.0 a 1.0).
            float GetLeftTrigger() const;
            // Retorna o valor do gatilho direito (0.0 a 1.0).
            float GetRightTrigger() const;
            // Verifica se o botão do ombro esquerdo (LB/L1) está pressionado.
            bool IsLeftShoulderPressed() const;
            // Verifica se o botão do ombro direito (RB/R1) está pressionado.
            bool IsRightShoulderPressed() const;
            // Verifica se o gatilho esquerdo está pressionado além de um certo limite (threshold).
            bool IsLeftTriggerPressed(float threshold = 0.5f) const;
            // Verifica se o gatilho direito está pressionado além de um certo limite (threshold).
            bool IsRightTriggerPressed(float threshold = 0.5f) const;
            // Verifica se o D-Pad para cima está pressionado.
            bool IsDPadUp() const;
            // Verifica se o D-Pad para a direita está pressionado.
            bool IsDPadRight() const;
            // Verifica se o D-Pad para baixo está pressionado.
            bool IsDPadDown() const;
            // Verifica se o D-Pad para a esquerda está pressionado.
            bool IsDPadLeft() const;
            // Verifica se o stick esquerdo (L3) está pressionado como um botão.
            bool IsLeftThumbstickPressed() const;
            // Verifica se o stick direito (R3) está pressionado como um botão.
            bool IsRightThumbstickPressed() const;

            // --- NOVO: Método para fazer o gamepad vibrar ---
            // Define a intensidade da vibração dos motores esquerdo e direito.
            // leftMotor e rightMotor devem estar no intervalo de 0.0 (sem vibração) a 1.0 (vibração máxima).
            // Nota: Requer GLFW 3.3 ou superior para funcionar.
            void SetVibration(float leftMotor, float rightMotor);

        private:
            int m_gamepadID;                 // ID interno do gamepad no GLFW.
            GLFWgamepadstate m_currentState;  // Estado atual dos botões e eixos do gamepad.
            GLFWgamepadstate m_previousState; // Estado anterior dos botões e eixos (para detecção de "just pressed/released").

            // Função auxiliar para copiar o estado de um gamepad.
            void CopyState(GLFWgamepadstate& dest, const GLFWgamepadstate& src);
        };

    } // namespace Input
} // namespace Luna
