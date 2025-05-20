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

    // Inicializa o renderer (pode ser vazio por enquanto, ou configurar coisas b�sicas)
    bool Init();

    // Prepara o frame para renderiza��o (limpa buffers)
    void prepareFrame();

    // Desenha um objeto (exemplo b�sico, voc� expandir� isso)
    void draw(Shader& shader, Texture& texture);

    // Apresenta o frame na tela
    void presentFrame(GLFWwindow* window);
};

#endif // RENDERER_HPP
