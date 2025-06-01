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

bool Keyboard::WasKeyJustPressed(int key) const {
    return keyStates.at(key) && !previousKeyStates.at(key);
}

bool Keyboard::WasKeyJustReleased(int key) const {
    return !keyStates.at(key) && previousKeyStates.at(key);
}

void Keyboard::EndFrame() {
    // Você pode manter os estados como estão, ou mover tudo para previous se quiser simplificar.
    // Neste caso, nada precisa ser feito, pois o UpdateKeyState já copia current -> previous.
}
