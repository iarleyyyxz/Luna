#include "Renderer.hpp"
#include <iostream>

Renderer::Renderer() {
    std::cout << "Renderer constru�do!" << std::endl;
}

Renderer::~Renderer() {
    std::cout << "Renderer destru�do!" << std::endl;
}

bool Renderer::Init() {
    // Configura��es b�sicas de renderiza��o OpenGL
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
    // Exemplo b�sico de desenho:
    // Voc� precisaria de um VAO/VBO aqui para desenhar algo real.
    // Por enquanto, este m�todo � apenas um placeholder.
    shader.use();
    texture.bind();
    // glDrawArrays ou glDrawElements aqui
    texture.unbind();
    shader.unuse();
}

void Renderer::presentFrame(GLFWwindow* window) {
    glfwSwapBuffers(window);
}
