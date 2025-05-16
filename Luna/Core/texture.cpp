#include "texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <GL/stb_image.h>
#include <iostream>

Texture::Texture() : m_textureID(0), m_width(0), m_height(0), m_bpp(0) {}

Texture::~Texture() {
    glDeleteTextures(1, &m_textureID);
}

bool Texture::loadFromFile(const std::string& path) {
    stbi_set_flip_vertically_on_load(1); // OpenGL espera a origem na parte inferior esquerda
    unsigned char* data = stbi_load(path.c_str(), &m_width, &m_height, &m_bpp, 4); // Carrega como RGBA

    if (!data) {
        std::cerr << "Failed to load texture: " << path << std::endl;
        return false;
    }

    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);

    // Configurar parâmetros da textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    return true;
}

void Texture::bind(unsigned int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}