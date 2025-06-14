#include "Gamepad.hpp" // Inclui o cabe�alho da classe Gamepad.
#include <cstring>     // Para std::memcpy (copia blocos de mem�ria).
#include <epsilon.hpp> // Para glm::clamp (garante que os valores estejam dentro de um intervalo).

namespace Luna {
    namespace Input {

        // Construtor: Inicializa os IDs e zera os estados atual e anterior do gamepad.
        Gamepad::Gamepad(int id) : m_gamepadID(id) {
            std::memset(&m_currentState, 0, sizeof(GLFWgamepadstate));
            std::memset(&m_previousState, 0, sizeof(GLFWgamepadstate));
            std::cout << "Gamepad object created for ID: " << m_gamepadID << std::endl;
        }

        // Helper para copiar o estado de um gamepad.
        // Isso � usado para armazenar o estado anterior antes de ler o novo.
        void Gamepad::CopyState(GLFWgamepadstate& dest, const GLFWgamepadstate& src) {
            std::memcpy(dest.buttons, src.buttons, sizeof(src.buttons)); // Copia os estados dos bot�es.
            std::memcpy(dest.axes, src.axes, sizeof(src.axes));         // Copia os estados dos eixos.
        }

        // Atualiza o estado do gamepad.
        // Primeiro, move o estado atual para o estado anterior.
        // Depois, tenta ler o novo estado do gamepad.
        bool Gamepad::Update() {
            CopyState(m_previousState, m_currentState); // Salva o estado atual como anterior.

            // Tenta obter o estado do gamepad.
            // glfwGetGamepadState retorna false se o joystick n�o for um gamepad compat�vel ou n�o estiver conectado.
            if (glfwGetGamepadState(m_gamepadID, &m_currentState)) {
                return true; // Gamepad conectado e estado lido com sucesso.
            }
            else {
                // Se a leitura falhou (desconectado ou n�o � um gamepad), reseta o estado atual.
                std::memset(&m_currentState, 0, sizeof(GLFWgamepadstate));
                return false; // Gamepad n�o conectado ou n�o compat�vel.
            }
        }

        // Verifica se o gamepad est� conectado e � um gamepad padr�o.
        bool Gamepad::IsConnected() const {
            // � importante verificar ambos: se o joystick est� presente E se ele � um gamepad.
            return glfwJoystickPresent(m_gamepadID) && glfwJoystickIsGamepad(m_gamepadID);
        }

        // Retorna o nome do gamepad.
        std::string Gamepad::GetName() const {
            const char* name = glfwGetGamepadName(m_gamepadID);
            if (name) {
                return std::string(name); // Retorna o nome se dispon�vel.
            }
            return "Unknown Gamepad"; // Retorna um nome padr�o se n�o houver.
        }

        // --- M�todos de Consulta de Bot�es Gen�ricos ---
        // Verifica se um bot�o espec�fico est� pressionado.
        bool Gamepad::IsButtonPressed(int button) const {
            if (button >= 0 && button <= GLFW_GAMEPAD_BUTTON_LAST) {
                return m_currentState.buttons[button] == GLFW_PRESS; // Verifica o estado atual do bot�o.
            }
            return false; // Bot�o inv�lido.
        }

        // Verifica se um bot�o foi acabado de pressionar.
        bool Gamepad::IsButtonJustPressed(int button) const {
            if (button >= 0 && button <= GLFW_GAMEPAD_BUTTON_LAST) {
                return m_currentState.buttons[button] == GLFW_PRESS &&
                    m_previousState.buttons[button] == GLFW_RELEASE; // Pressionado agora, estava solto antes.
            }
            return false; // Bot�o inv�lido.
        }

        // Verifica se um bot�o foi acabado de soltar.
        bool Gamepad::IsButtonJustReleased(int button) const {
            if (button >= 0 && button <= GLFW_GAMEPAD_BUTTON_LAST) {
                return m_currentState.buttons[button] == GLFW_RELEASE &&
                    m_previousState.buttons[button] == GLFW_PRESS; // Solto agora, estava pressionado antes.
            }
            return false; // Bot�o inv�lido.
        }

        // --- M�todos de Consulta de Eixos Gen�ricos ---
        // Retorna o valor de um eixo espec�fico.
        float Gamepad::GetAxisValue(int axis) const {
            if (axis >= 0 && axis <= GLFW_GAMEPAD_AXIS_LAST) {
                return m_currentState.axes[axis]; // Retorna o valor do eixo.
            }
            return 0.0f; // Eixo inv�lido.
        }

        // --- M�todos de Consulta de Conveni�ncia (Espec�ficos para Gamepads Padr�o) ---
        // Retorna o vetor do stick esquerdo.
        glm::vec2 Gamepad::GetLeftStick() const {
            return glm::vec2(GetAxisValue(GLFW_GAMEPAD_AXIS_LEFT_X), GetAxisValue(GLFW_GAMEPAD_AXIS_LEFT_Y));
        }

        // Retorna o vetor do stick direito.
        glm::vec2 Gamepad::GetRightStick() const {
            return glm::vec2(GetAxisValue(GLFW_GAMEPAD_AXIS_RIGHT_X), GetAxisValue(GLFW_GAMEPAD_AXIS_RIGHT_Y));
        }

        // Retorna o valor do gatilho esquerdo (normalizado de 0.0 a 1.0).
        float Gamepad::GetLeftTrigger() const {
            // Gatilhos GLFW variam de -1.0 (n�o pressionado) a 1.0 (totalmente pressionado).
            // Convertemos para 0.0 a 1.0 para uso mais intuitivo.
            return (GetAxisValue(GLFW_GAMEPAD_AXIS_LEFT_TRIGGER) + 1.0f) / 2.0f;
        }

        // Retorna o valor do gatilho direito (normalizado de 0.0 a 1.0).
        float Gamepad::GetRightTrigger() const {
            // Gatilhos GLFW variam de -1.0 (n�o pressionado) a 1.0 (totalmente pressionado).
            return (GetAxisValue(GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER) + 1.0f) / 2.0f;
        }

        // Verifica se o bot�o do ombro esquerdo est� pressionado.
        bool Gamepad::IsLeftShoulderPressed() const {
            return IsButtonPressed(GLFW_GAMEPAD_BUTTON_LEFT_BUMPER);
        }

        // Verifica se o bot�o do ombro direito est� pressionado.
        bool Gamepad::IsRightShoulderPressed() const {
            return IsButtonPressed(GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER);
        }

        // Verifica se o gatilho esquerdo est� pressionado al�m de um limite.
        bool Gamepad::IsLeftTriggerPressed(float threshold) const {
            return GetLeftTrigger() > threshold;
        }

        // Verifica se o gatilho direito est� pressionado al�m de um limite.
        bool Gamepad::IsRightTriggerPressed(float threshold) const {
            return GetRightTrigger() > threshold;
        }

        // Verifica se o D-Pad para cima est� pressionado.
        bool Gamepad::IsDPadUp() const {
            return IsButtonPressed(GLFW_GAMEPAD_BUTTON_DPAD_UP);
        }

        // Verifica se o D-Pad para a direita est� pressionado.
        bool Gamepad::IsDPadRight() const {
            return IsButtonPressed(GLFW_GAMEPAD_BUTTON_DPAD_RIGHT);
        }

        // Verifica se o D-Pad para baixo est� pressionado.
        bool Gamepad::IsDPadDown() const {
            return IsButtonPressed(GLFW_GAMEPAD_BUTTON_DPAD_DOWN);
        }

        // Verifica se o D-Pad para a esquerda est� pressionado.
        bool Gamepad::IsDPadLeft() const {
            return IsButtonPressed(GLFW_GAMEPAD_BUTTON_DPAD_LEFT);
        }

        // Verifica se o stick esquerdo est� pressionado.
        bool Gamepad::IsLeftThumbstickPressed() const {
            return IsButtonPressed(GLFW_GAMEPAD_BUTTON_LEFT_THUMB);
        }

        // Verifica se o stick direito est� pressionado.
        bool Gamepad::IsRightThumbstickPressed() const {
            return IsButtonPressed(GLFW_GAMEPAD_BUTTON_RIGHT_THUMB);
        }

        // --- NOVO: Implementa��o do m�todo de vibra��o com verifica��o de vers�o ---
        // Define a intensidade da vibra��o dos motores esquerdo e direito do gamepad.
        // leftMotor e rightMotor s�o valores de 0.0 a 1.0.
        void Gamepad::SetVibration(float leftMotor, float rightMotor) {
            // Verifica se a vers�o da GLFW suporta vibra��o (GLFW 3.3 ou superior).
#if GLFW_VERSION_MAJOR > 3 || (GLFW_VERSION_MAJOR == 3 && GLFW_VERSION_MINOR >= 3)
            if (IsConnected()) {
                // Garante que os valores estejam dentro do intervalo [0.0, 1.0].
                leftMotor = glm::clamp(leftMotor, 0.0f, 1.0f);
                rightMotor = glm::clamp(rightMotor, 0.0f, 1.0f);

                // Define a vibra��o usando a fun��o GLFW.
                // O motor esquerdo geralmente controla vibra��es de baixa frequ�ncia (pesadas).
                // O motor direito geralmente controla vibra��es de alta frequ�ncia (leves).
                glfwSetJoystickVibration(m_gamepadID, leftMotor, rightMotor);
                std::cout << "Ajustando vibra��o para o Gamepad ID " << m_gamepadID
                    << ": Esquerdo=" << leftMotor << ", Direito=" << rightMotor << std::endl;
            }
            else {
                std::cout << "N�o foi poss�vel ajustar a vibra��o: Gamepad ID " << m_gamepadID << " n�o conectado." << std::endl;
            }
#else
    // Mensagem de aviso se a vers�o da GLFW for muito antiga.
            std::cerr << "Aviso: A vibra��o do gamepad requer GLFW 3.3 ou superior. A sua vers�o GLFW � "
                << GLFW_VERSION_MAJOR << "." << GLFW_VERSION_MINOR << ". A fun��o SetVibration n�o est� dispon�vel." << std::endl;
#endif
        }

    } // namespace Input
} // namespace Luna
