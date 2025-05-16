#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

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

    // Inicializar GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Configurar o viewport OpenGL
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

   

    // Loop principal
    while (!glfwWindowShouldClose(window)) {
        // Limpar o buffer de cor
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // Trocar os buffers
        glfwSwapBuffers(window);

        // Processar eventos
        glfwPollEvents();
    }

    // Encerrar o GLFW
    glfwTerminate();
    return 0;
}