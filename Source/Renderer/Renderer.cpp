#include "Renderer.hpp"
#include <iostream>

Renderer::Renderer() {
    std::cout << "Renderer construído!" << std::endl;
}

Renderer::~Renderer() {
    std::cout << "Renderer destruído!" << std::endl;
}

bool Renderer::Init() {
    // Configurações básicas de renderização OpenGL
    glEnable(GL_DEPTH_TEST); // Habilitar teste de profundidade
    std::cout << "Renderer inicializado." << std::endl;
    return true;
}

void Renderer::prepareFrame() {
    // Limpar o buffer de cor e profundidade
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Cor de fundo
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::draw(Shader& shader, Texture& texture) {
    // Exemplo básico de desenho:
    // Você precisaria de um VAO/VBO aqui para desenhar algo real.
    // Por enquanto, este método é apenas um placeholder.
    shader.use();
    texture.bind();
    // glDrawArrays ou glDrawElements aqui
    texture.unbind();
    shader.unuse();
}

void Renderer::presentFrame(GLFWwindow* window) {
    glfwSwapBuffers(window);
}
