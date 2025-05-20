#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.hpp"
#include "Texture.hpp"

class Renderer {
public:
    Renderer();
    ~Renderer();

    // Inicializa o renderer (pode ser vazio por enquanto, ou configurar coisas básicas)
    bool Init();

    // Prepara o frame para renderização (limpa buffers)
    void prepareFrame();

    // Desenha um objeto (exemplo básico, você expandirá isso)
    void draw(Shader& shader, Texture& texture);

    // Apresenta o frame na tela
    void presentFrame(GLFWwindow* window);
};

#endif // RENDERER_HPP
