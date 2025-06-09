// Keyboard.cpp
#include "Keyboard.hpp"
#include <GLFW/glfw3.h>

void Keyboard::UpdateKeyState(int key, int action) {
    previousKeyStates[key] = keyStates[key];
    if (action == GLFW_PRESS) {
        keyStates[key] = true;
    }
    else if (action == GLFW_RELEASE) {
        keyStates[key] = false;
    }
}

bool Keyboard::IsKeyPressed(int key) const {
    auto it = keyStates.find(key);
    return it != keyStates.end() && it->second;
}

// *** NOVO / EQUIVALENTE: Implementa��o expl�cita do IsKeyDown ***
bool Keyboard::IsKeyDown(int key) const {
    // Apenas para garantir que a chave � encontrada, caso contr�rio, retorna false
    // (ou use .at() se tiver certeza que a chave existe e quer uma exce��o para depura��o)
    auto it = keyStates.find(key);
    return it != keyStates.end() && it->second;
}

bool Keyboard::WasKeyJustPressed(int key) const {
    return keyStates.at(key) && !previousKeyStates.at(key);
}

bool Keyboard::WasKeyJustReleased(int key) const {
    return !keyStates.at(key) && previousKeyStates.at(key);
}

void Keyboard::EndFrame() {
    // Voc� pode manter os estados como est�o, ou mover tudo para previous se quiser simplificar.
    // Neste caso, nada precisa ser feito, pois o UpdateKeyState j� copia current -> previous.
}
