#ifndef INPUT_MANAGER_HPP
#define INPUT_MANAGER_HPP

#include <GLFW/glfw3.h>
#include <unordered_map>

class Keyboard {
public:
    // Use um mapa para rastrear o estado das teclas
    std::unordered_map<int, bool> keyStates;

    // Construtor
    Keyboard();

    // Destrutor
    ~Keyboard();

    // Função para atualizar o estado de uma tecla
    void UpdateKeyState(int key, int action);

    // Função para verificar se uma tecla está pressionada
    bool IsKeyPressed(int key);

    // Função para verificar se uma tecla foi pressionada uma vez
    bool IsKeyJustPressed(int key); //Adicionado

private:
    std::unordered_map<int, bool> previousKeyStates;
};

class Mouse {
public:
    // Construtor
    Mouse();

    // Destrutor
    ~Mouse();
    // Posição atual do mouse
    double X;
    double Y;

    // Delta de movimento do mouse
    double DeltaX;
    double DeltaY;

    // Estado dos botões do mouse
    std::unordered_map<int, bool> ButtonStates;

    // Função para atualizar a posição do mouse e o delta
    void UpdatePosition(GLFWwindow* window);
    void UpdateButtonState(int button, int action);

    bool IsButtonPressed(int button);
    bool IsButtonJustPressed(int button); // Adicionado
private:
    double PreviousX;
    double PreviousY;
    std::unordered_map<int, bool> PreviousButtonStates;
};

#endif // INPUT_MANAGER_HPP