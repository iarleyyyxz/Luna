#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Renderer/shader.hpp"
#include "Renderer/texture.hpp"
#include "camera.hpp"
#include "Renderer/sprite.hpp"

#include <Core/keyboard.hpp>
#include <Core/mouse.hpp>
#include <Core/Game.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    // Inicializar o GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    // Definir as hints da janela
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Para macOS
#endif

    // Criar a janela GLFW
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Luna", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Tornar o contexto da janela o atual
    glfwMakeContextCurrent(window);

    // Inicializar GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Configurar o viewport OpenGL
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Core::MouseListener mouseListener;
    Core::KeyListener keyListener;

    glfwSetWindowUserPointer(window, &keyListener);
    glfwSetKeyCallback(window, Core::KeyListener::glfwKeyCallback);

    glfwSetWindowUserPointer(window, &mouseListener);
    glfwSetMouseButtonCallback(window, Core::MouseListener::mouseButtonCallback);
    glfwSetCursorPosCallback(window, Core::MouseListener::cursorPosCallback);
    glfwSetScrollCallback(window, Core::MouseListener::mouseScrollCallback);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    Core::Game game(window, &keyListener, &mouseListener); // Passar a janela para Game

    // Loop principal do jogo
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        keyListener.endFrame();
        mouseListener.endFrame();

        // Renderizar a cena do jogo (usando a instância de Game)
        game.render();

       

        glfwSwapBuffers(window);
    }


    glfwTerminate();
    return 0;
}
