#include "Texture.hpp"
#include <iostream>

// IMPORTANTE: Esta linha DEVE estar em APENAS UM ficheiro .cpp no seu projeto
// que inclui "stb_image.h". Geralmente, é o ficheiro .cpp da classe Texture
// ou um ficheiro de utilitários de imagem.
#define STB_IMAGE_IMPLEMENTATION
#include "gl/stb_image.h" // Incluir a biblioteca stb_image

// Construtor para carregar uma textura a partir de um ficheiro de imagem
Texture::Texture(const char* imagePath) : ID(0), width(0), height(0), nrChannels(0), path(imagePath) {
    // Carregar dados da imagem usando stb_image
    stbi_set_flip_vertically_on_load(true); // Inverter a imagem no eixo Y (OpenGL espera o Y de baixo para cima)
    unsigned char* data = stbi_load(imagePath, &width, &height, &nrChannels, 0);

    if (data) {
        // Determinar o formato OpenGL com base no número de canais
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;
        else {
            std::cerr << "ERRO::TEXTURA::FORMATO_NAO_SUPORTADO: " << imagePath << " - Canais: " << nrChannels << std::endl;
            stbi_image_free(data); // Liberar a memória da imagem
            return;
        }

        // Gerar e vincular a textura OpenGL
        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_2D, ID);

        // Definir opções de envolvimento (wrapping) e filtragem da textura
        // GL_REPEAT: Repete a textura se as coordenadas estiverem fora do [0,1]
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // NOVO: Usar GL_NEAREST para filtragem de minificação e magnificação para um visual pixelado
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST); // Para minificação (redução)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // Para magnificação (ampliação)

        // Carregar os dados da imagem para a textura OpenGL
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D); // Gerar mipmaps para otimização de diferentes distâncias

        std::cout << "Textura carregada com sucesso: " << imagePath << std::endl;
    }
    else {
        std::cerr << "ERRO::TEXTURA::FALHA_AO_CARREGAR: " << imagePath << " - " << stbi_failure_reason() << std::endl;
    }

    stbi_image_free(data); // Liberar a memória da imagem carregada pela stb_image
}

// Construtor para criar uma textura a partir de dados de pixel brutos (por exemplo, para uma textura branca 1x1)
Texture::Texture(int w, int h, unsigned char* data, GLenum format) : ID(0), width(w), height(h), nrChannels(format == GL_RGBA ? 4 : (format == GL_RGB ? 3 : 1)), path("") {
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    // Para texturas geradas (como a branca padrão), geralmente usamos CLAMP_TO_EDGE
    // para evitar problemas de amostragem nas bordas.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // NOVO: Usar GL_NEAREST para filtragem de minificação e magnificação para um visual pixelado
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // Filtragem por vizinho mais próximo
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // Filtragem por vizinho mais próximo

    // Carregar os dados de pixel brutos para a textura
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    // Não geramos mipmaps automaticamente aqui, pois para texturas 1x1 ou geradas, raramente são necessários.

    std::cout << "Textura gerada a partir de dados brutos (" << width << "x" << height << ") com sucesso." << std::endl;
}

// Vincula a textura à unidade de textura especificada
void Texture::bind(GLenum textureUnit) {
    glActiveTexture(textureUnit); // Ativa a unidade de textura
    glBindTexture(GL_TEXTURE_2D, ID); // Vincula a textura a essa unidade
}

// Desvincula a textura (vincula a textura 0, que é a textura padrão nula)
void Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}
