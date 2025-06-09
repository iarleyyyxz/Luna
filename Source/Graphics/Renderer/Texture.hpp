#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <GL/glew.h>
#include <string>

class Texture {
public:
    GLuint ID;
    int width, height, nrChannels;
    std::string path; // Pode ser vazio para texturas geradas

    // Construtor que carrega e gera uma textura a partir de um ficheiro de imagem
    Texture(const char* path);

    // NOVO: Construtor para criar uma textura a partir de dados de pixel brutos
    // Útil para texturas geradas em tempo de execução (como a textura branca padrão)
    Texture(int width, int height, unsigned char* data, GLenum format = GL_RGBA);


    // Vincula a textura a uma unidade de textura ativa
    void bind(GLenum textureUnit = GL_TEXTURE0);
    // Desvincula a textura
    void unbind();

    // Retorna o ID OpenGL da textura
    GLuint getID() const { return ID; }
};

#endif // TEXTURE_HPP