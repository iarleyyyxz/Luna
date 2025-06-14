#include "Gamepad.hpp" // Inclui o cabeçalho da classe Gamepad.
#include <cstring>     // Para std::memcpy (copia blocos de memória).
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
        // Isso é usado para armazenar o estado anterior antes de ler o novo.
        void Gamepad::CopyState(GLFWgamepadstate& dest, const GLFWgamepadstate& src) {
            std::memcpy(dest.buttons, src.buttons, sizeof(src.buttons)); // Copia os estados dos botões.
            std::memcpy(dest.axes, src.axes, sizeof(src.axes));         // Copia os estados dos eixos.
        }

        // Atualiza o estado do gamepad.
        // Primeiro, move o estado atual para o estado anterior.
        // Depois, tenta ler o novo estado do gamepad.
        bool Gamepad::Update() {
            CopyState(m_previousState, m_currentState); // Salva o estado atual como anterior.

            // Tenta obter o estado do gamepad.
            // glfwGetGamepadState retorna false se o joystick não for um gamepad compatível ou não estiver conectado.
            if (glfwGetGamepadState(m_gamepadID, &m_currentState)) {
                return true; // Gamepad conectado e estado lido com sucesso.
            }
            else {
                // Se a leitura falhou (desconectado ou não é um gamepad), reseta o estado atual.
                std::memset(&m_currentState, 0, sizeof(GLFWgamepadstate));
                return false; // Gamepad não conectado ou não compatível.
            }
        }

        // Verifica se o gamepad está conectado e é um gamepad padrão.
        bool Gamepad::IsConnected() const {
            // É importante verificar ambos: se o joystick está presente E se ele é um gamepad.
            return glfwJoystickPresent(m_gamepadID) && glfwJoystickIsGamepad(m_gamepadID);
        }

        // Retorna o nome do gamepad.
        std::string Gamepad::GetName() const {
            const char* name = glfwGetGamepadName(m_gamepadID);
            if (name) {
                return std::string(name); // Retorna o nome se disponível.
            }
            return "Unknown Gamepad"; // Retorna um nome padrão se não houver.
        }

        // --- Métodos de Consulta de Botões Genéricos ---
        // Verifica se um botão específico está pressionado.
        bool Gamepad::IsButtonPressed(int button) const {
            if (button >= 0 && button <= GLFW_GAMEPAD_BUTTON_LAST) {
                return m_currentState.buttons[button] == GLFW_PRESS; // Verifica o estado atual do botão.
            }
            return false; // Botão inválido.
        }

        // Verifica se um botão foi acabado de pressionar.
        bool Gamepad::IsButtonJustPressed(int button) const {
            if (button >= 0 && button <= GLFW_GAMEPAD_BUTTON_LAST) {
                return m_currentState.buttons[button] == GLFW_PRESS &&
                    m_previousState.buttons[button] == GLFW_RELEASE; // Pressionado agora, estava solto antes.
            }
            return false; // Botão inválido.
        }

        // Verifica se um botão foi acabado de soltar.
        bool Gamepad::IsButtonJustReleased(int button) const {
            if (button >= 0 && button <= GLFW_GAMEPAD_BUTTON_LAST) {
                return m_currentState.buttons[button] == GLFW_RELEASE &&
                    m_previousState.buttons[button] == GLFW_PRESS; // Solto agora, estava pressionado antes.
            }
            return false; // Botão inválido.
        }

        // --- Métodos de Consulta de Eixos Genéricos ---
        // Retorna o valor de um eixo específico.
        float Gamepad::GetAxisValue(int axis) const {
            if (axis >= 0 && axis <= GLFW_GAMEPAD_AXIS_LAST) {
                return m_currentState.axes[axis]; // Retorna o valor do eixo.
            }
            return 0.0f; // Eixo inválido.
        }

        // --- Métodos de Consulta de Conveniência (Específicos para Gamepads Padrão) ---
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
            // Gatilhos GLFW variam de -1.0 (não pressionado) a 1.0 (totalmente pressionado).
            // Convertemos para 0.0 a 1.0 para uso mais intuitivo.
            return (GetAxisValue(GLFW_GAMEPAD_AXIS_LEFT_TRIGGER) + 1.0f) / 2.0f;
        }

        // Retorna o valor do gatilho direito (normalizado de 0.0 a 1.0).
        float Gamepad::GetRightTrigger() const {
            // Gatilhos GLFW variam de -1.0 (não pressionado) a 1.0 (totalmente pressionado).
            return (GetAxisValue(GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER) + 1.0f) / 2.0f;
        }

        // Verifica se o botão do ombro esquerdo está pressionado.
        bool Gamepad::IsLeftShoulderPressed() const {
            return IsButtonPressed(GLFW_GAMEPAD_BUTTON_LEFT_BUMPER);
        }

        // Verifica se o botão do ombro direito está pressionado.
        bool Gamepad::IsRightShoulderPressed() const {
            return IsButtonPressed(GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER);
        }

        // Verifica se o gatilho esquerdo está pressionado além de um limite.
        bool Gamepad::IsLeftTriggerPressed(float threshold) const {
            return GetLeftTrigger() > threshold;
        }

        // Verifica se o gatilho direito está pressionado além de um limite.
        bool Gamepad::IsRightTriggerPressed(float threshold) const {
            return GetRightTrigger() > threshold;
        }

        // Verifica se o D-Pad para cima está pressionado.
        bool Gamepad::IsDPadUp() const {
            return IsButtonPressed(GLFW_GAMEPAD_BUTTON_DPAD_UP);
        }

        // Verifica se o D-Pad para a direita está pressionado.
        bool Gamepad::IsDPadRight() const {
            return IsButtonPressed(GLFW_GAMEPAD_BUTTON_DPAD_RIGHT);
        }

        // Verifica se o D-Pad para baixo está pressionado.
        bool Gamepad::IsDPadDown() const {
            return IsButtonPressed(GLFW_GAMEPAD_BUTTON_DPAD_DOWN);
        }

        // Verifica se o D-Pad para a esquerda está pressionado.
        bool Gamepad::IsDPadLeft() const {
            return IsButtonPressed(GLFW_GAMEPAD_BUTTON_DPAD_LEFT);
        }

        // Verifica se o stick esquerdo está pressionado.
        bool Gamepad::IsLeftThumbstickPressed() const {
            return IsButtonPressed(GLFW_GAMEPAD_BUTTON_LEFT_THUMB);
        }

        // Verifica se o stick direito está pressionado.
        bool Gamepad::IsRightThumbstickPressed() const {
            return IsButtonPressed(GLFW_GAMEPAD_BUTTON_RIGHT_THUMB);
        }

        // --- NOVO: Implementação do método de vibração com verificação de versão ---
        // Define a intensidade da vibração dos motores esquerdo e direito do gamepad.
        // leftMotor e rightMotor são valores de 0.0 a 1.0.
        void Gamepad::SetVibration(float leftMotor, float rightMotor) {
            // Verifica se a versão da GLFW suporta vibração (GLFW 3.3 ou superior).
#if GLFW_VERSION_MAJOR > 3 || (GLFW_VERSION_MAJOR == 3 && GLFW_VERSION_MINOR >= 3)
            if (IsConnected()) {
                // Garante que os valores estejam dentro do intervalo [0.0, 1.0].
                leftMotor = glm::clamp(leftMotor, 0.0f, 1.0f);
                rightMotor = glm::clamp(rightMotor, 0.0f, 1.0f);

                // Define a vibração usando a função GLFW.
                // O motor esquerdo geralmente controla vibrações de baixa frequência (pesadas).
                // O motor direito geralmente controla vibrações de alta frequência (leves).
                glfwSetJoystickVibration(m_gamepadID, leftMotor, rightMotor);
                std::cout << "Ajustando vibração para o Gamepad ID " << m_gamepadID
                    << ": Esquerdo=" << leftMotor << ", Direito=" << rightMotor << std::endl;
            }
            else {
                std::cout << "Não foi possível ajustar a vibração: Gamepad ID " << m_gamepadID << " não conectado." << std::endl;
            }
#else
    // Mensagem de aviso se a versão da GLFW for muito antiga.
            std::cerr << "Aviso: A vibração do gamepad requer GLFW 3.3 ou superior. A sua versão GLFW é "
                << GLFW_VERSION_MAJOR << "." << GLFW_VERSION_MINOR << ". A função SetVibration não está disponível." << std::endl;
#endif
        }

    } // namespace Input
} // namespace Luna
