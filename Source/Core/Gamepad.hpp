#pragma once

#include <GLFW/glfw3.h> // Inclui a biblioteca GLFW para manipula��o de gamepad.
#include <glm.hpp>  // Inclui GLM para tipos de vetor (como glm::vec2).
#include <string>       // Inclui para uso de std::string.
#include <iostream>     // Inclui para sa�da de console (std::cout).

namespace Luna {
    namespace Input {

        class Gamepad {
        public:
            // Construtor: Inicializa o gamepad com um ID espec�fico.
            Gamepad(int id);

            // Atualiza o estado do gamepad (bot�es e eixos). Retorna true se o estado foi atualizado com sucesso, false caso contr�rio.
            bool Update();
            // Verifica se o gamepad est� conectado e � reconhecido como um gamepad padr�o.
            bool IsConnected() const;
            // Retorna o nome do gamepad.
            std::string GetName() const;

            // --- M�todos de Consulta de Bot�es Gen�ricos ---
            // Verifica se um bot�o espec�fico est� pressionado.
            bool IsButtonPressed(int button) const;
            // Verifica se um bot�o foi *acabado de* pressionar (estava solto e agora est� pressionado).
            bool IsButtonJustPressed(int button) const;
            // Verifica se um bot�o foi *acabado de* soltar (estava pressionado e agora est� solto).
            bool IsButtonJustReleased(int button) const;

            // --- M�todos de Consulta de Eixos Gen�ricos ---
            // Retorna o valor de um eixo espec�fico (varia de -1.0 a 1.0).
            float GetAxisValue(int axis) const;

            // --- M�todos de Conveni�ncia (Espec�ficos para Gamepads Padr�o) ---
            // Retorna o vetor (x, y) do stick esquerdo.
            glm::vec2 GetLeftStick() const;
            // Retorna o vetor (x, y) do stick direito.
            glm::vec2 GetRightStick() const;
            // Retorna o valor do gatilho esquerdo (0.0 a 1.0).
            float GetLeftTrigger() const;
            // Retorna o valor do gatilho direito (0.0 a 1.0).
            float GetRightTrigger() const;
            // Verifica se o bot�o do ombro esquerdo (LB/L1) est� pressionado.
            bool IsLeftShoulderPressed() const;
            // Verifica se o bot�o do ombro direito (RB/R1) est� pressionado.
            bool IsRightShoulderPressed() const;
            // Verifica se o gatilho esquerdo est� pressionado al�m de um certo limite (threshold).
            bool IsLeftTriggerPressed(float threshold = 0.5f) const;
            // Verifica se o gatilho direito est� pressionado al�m de um certo limite (threshold).
            bool IsRightTriggerPressed(float threshold = 0.5f) const;
            // Verifica se o D-Pad para cima est� pressionado.
            bool IsDPadUp() const;
            // Verifica se o D-Pad para a direita est� pressionado.
            bool IsDPadRight() const;
            // Verifica se o D-Pad para baixo est� pressionado.
            bool IsDPadDown() const;
            // Verifica se o D-Pad para a esquerda est� pressionado.
            bool IsDPadLeft() const;
            // Verifica se o stick esquerdo (L3) est� pressionado como um bot�o.
            bool IsLeftThumbstickPressed() const;
            // Verifica se o stick direito (R3) est� pressionado como um bot�o.
            bool IsRightThumbstickPressed() const;

            // --- NOVO: M�todo para fazer o gamepad vibrar ---
            // Define a intensidade da vibra��o dos motores esquerdo e direito.
            // leftMotor e rightMotor devem estar no intervalo de 0.0 (sem vibra��o) a 1.0 (vibra��o m�xima).
            // Nota: Requer GLFW 3.3 ou superior para funcionar.
            void SetVibration(float leftMotor, float rightMotor);

        private:
            int m_gamepadID;                 // ID interno do gamepad no GLFW.
            GLFWgamepadstate m_currentState;  // Estado atual dos bot�es e eixos do gamepad.
            GLFWgamepadstate m_previousState; // Estado anterior dos bot�es e eixos (para detec��o de "just pressed/released").

            // Fun��o auxiliar para copiar o estado de um gamepad.
            void CopyState(GLFWgamepadstate& dest, const GLFWgamepadstate& src);
        };

    } // namespace Input
} // namespace Luna
