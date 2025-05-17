#include "keyboard.hpp"
#include <algorithm>
#include <iostream>

namespace Core {

    KeyListener::KeyListener() {
        std::fill(keyPressed, keyPressed + GLFW_KEY_LAST + 1, false);
    }

    void KeyListener::endFrame() {
        // Lógica para o final do frame, se necessário
    }

    void KeyListener::glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        KeyListener* listener = static_cast<KeyListener*>(glfwGetWindowUserPointer(window));
        if (listener) {
            listener->processKeyEvent(key, action);
        }
    }

    void KeyListener::processKeyEvent(int key, int action) {
        if (key >= 0 && key <= GLFW_KEY_LAST) {
            if (action == GLFW_PRESS) {
                keyPressed[key] = true;
            }
            else if (action == GLFW_RELEASE) {
                keyPressed[key] = false;
            }
        }
    }

    bool KeyListener::isKeyPressed(int keyCode) const {
        if (keyCode >= 0 && keyCode <= GLFW_KEY_LAST) {
            return keyPressed[keyCode];
        }
        return false;
    }

} // namespace Core