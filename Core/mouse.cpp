#include "mouse.hpp"
#include <algorithm>

namespace Core {

    MouseListener::MouseListener() : currentPosition(0.0f), scrollDelta(0.0f) {
        std::fill(mouseButtonPressed, mouseButtonPressed + GLFW_MOUSE_BUTTON_LAST + 1, false);
    }

    void MouseListener::endFrame() {
        scrollDelta.x = 0.0f;
        scrollDelta.y = 0.0f;
    }

    void MouseListener::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        MouseListener* listener = static_cast<MouseListener*>(glfwGetWindowUserPointer(window));
        if (listener) {
            if (button >= 0 && button <= GLFW_MOUSE_BUTTON_LAST) {
                if (action == GLFW_PRESS) {
                    listener->mouseButtonPressed[button] = true;
                }
                else if (action == GLFW_RELEASE) {
                    listener->mouseButtonPressed[button] = false;
                }
            }
        }
    }

    void MouseListener::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
        MouseListener* listener = static_cast<MouseListener*>(glfwGetWindowUserPointer(window));
        if (listener) {
            listener->currentPosition.x = static_cast<float>(xpos);
            listener->currentPosition.y = static_cast<float>(ypos);
        }
    }

    void MouseListener::mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
        MouseListener* listener = static_cast<MouseListener*>(glfwGetWindowUserPointer(window));
        if (listener) {
            listener->scrollDelta.x += static_cast<float>(xoffset);
            listener->scrollDelta.y += static_cast<float>(yoffset);
        }
    }

    bool MouseListener::isButtonPressed(int button) const {
        if (button >= 0 && button <= GLFW_MOUSE_BUTTON_LAST) {
            return mouseButtonPressed[button];
        }
        return false;
    }

    glm::vec2 MouseListener::getPosition() const {
        return currentPosition;
    }

    glm::vec2 MouseListener::getScrollDelta() const {
        return scrollDelta;
    }

} // namespace Core