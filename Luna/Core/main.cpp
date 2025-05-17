// main.cpp
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


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main()
{
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


    Core::KeyListener keyListener;
    Core::MouseListener mouseListener;

    glfwSetWindowUserPointer(window, &keyListener);
    glfwSetKeyCallback(window, Core::KeyListener::keyCallback);

    glfwSetWindowUserPointer(window, &mouseListener);
    glfwSetMouseButtonCallback(window, Core::MouseListener::mouseButtonCallback);
    glfwSetCursorPosCallback(window, Core::MouseListener::cursorPosCallback);
    glfwSetScrollCallback(window, Core::MouseListener::mouseScrollCallback);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


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

    // Loop principal
    while (!glfwWindowShouldClose(window)) {
        // Limpar o buffer de cor
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Trocar os buffers
        glfwSwapBuffers(window);

        // Processar eventos
        glfwPollEvents();
    }
;

    glfwTerminate();
    return 0;
}

