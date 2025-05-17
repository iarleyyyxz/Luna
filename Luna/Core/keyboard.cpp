#include "keyboard.hpp"
#include <algorithm>

namespace Core {

    KeyListener::KeyListener() {
        std::fill(keyPressed, keyPressed + GLFW_KEY_LAST + 1, false);
    }

    void KeyListener::endFrame() {
        std::fill(keyPressed, keyPressed + GLFW_KEY_LAST + 1, false);
    }


    void KeyListener::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        KeyListener* listener = static_cast<KeyListener*>(glfwGetWindowUserPointer(window));
        if (listener) {
            if (key >= 0 && key <= GLFW_KEY_LAST) {
                if (action == GLFW_PRESS) {
                    listener->keyPressed[key] = true;
                }
                else if (action == GLFW_RELEASE) {
                    listener->keyPressed[key] = false;
                }
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