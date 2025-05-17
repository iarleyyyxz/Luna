#ifndef MOUSE_LISTENER_HPP
#define MOUSE_LISTENER_HPP

#include <GLFW/glfw3.h>
#include <glm.hpp>

namespace Core {

    class MouseListener {
    public:
        MouseListener();
        ~MouseListener() = default;

        bool mouseButtonPressed[GLFW_MOUSE_BUTTON_LAST + 1];
        glm::vec2 currentPosition;
        glm::vec2 scrollDelta;

        static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
        static void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
        bool isButtonPressed(int button) const;
        glm::vec2 getPosition() const;
        glm::vec2 getScrollDelta() const;
        void endFrame();
    };

} // namespace Core

#endif // MOUSE_LISTENER_HPP