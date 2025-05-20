#include "InputManager.hpp"
#include <iostream>

// --- Keyboard ---
Keyboard::Keyboard() {
    // Inicializa todas as teclas como não pressionadas
    for (int key = 0; key < GLFW_KEY_LAST; ++key) {
        keyStates[key] = false;
        previousKeyStates[key] = false;
    }
}

Keyboard::~Keyboard() {
}

void Keyboard::UpdateKeyState(int key, int action) {
    previousKeyStates[key] = keyStates[key];
    if (action == GLFW_PRESS) {
        keyStates[key] = true;
    }
    else if (action == GLFW_RELEASE) {
        keyStates[key] = false;
    }
}

bool Keyboard::IsKeyPressed(int key) {
    return keyStates[key];
}

bool Keyboard::IsKeyJustPressed(int key) {
    return keyStates[key] && !previousKeyStates[key];
}

// --- Mouse ---
Mouse::Mouse() : X(0.0), Y(0.0), PreviousX(0.0), PreviousY(0.0) {
    for (int button = 0; button < GLFW_MOUSE_BUTTON_LAST; ++button) {
        ButtonStates[button] = false;
        PreviousButtonStates[button] = false;
    }
}

Mouse::~Mouse() {
}

void Mouse::UpdatePosition(GLFWwindow* window) {
    PreviousX = X;
    PreviousY = Y;
    glfwGetCursorPos(window, &X, &Y);
    DeltaX = X - PreviousX;
    DeltaY = Y - PreviousY;
}

void Mouse::UpdateButtonState(int button, int action) {
    PreviousButtonStates[button] = ButtonStates[button];
    if (action == GLFW_PRESS) {
        ButtonStates[button] = true;
    }
    else if (action == GLFW_RELEASE) {
        ButtonStates[button] = false;
    }
}

bool Mouse::IsButtonPressed(int button) {
    return ButtonStates[button];
}

bool Mouse::IsButtonJustPressed(int button) {
    return ButtonStates[button] && !PreviousButtonStates[button];
}