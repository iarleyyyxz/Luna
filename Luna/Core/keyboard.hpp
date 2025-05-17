#ifndef KEY_LISTENER_HPP
#define KEY_LISTENER_HPP

#include <GLFW/glfw3.h>
#include <vector>

namespace Core {

    class KeyListener {
    public:
        KeyListener();
        ~KeyListener() = default;

        bool keyPressed[GLFW_KEY_LAST + 1];

        static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        void processKeyEvent(int key, int action);
        bool isKeyPressed(int keyCode) const;
        void endFrame();
    };

} // namespace Core

#endif // KEY_LISTENER_HPP