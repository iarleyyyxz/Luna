#include "ResourceManager.hpp"
#include <iostream>

ResourceManager::ResourceManager() {
    std::cout << "ResourceManager constru�do!" << std::endl;
}

ResourceManager::~ResourceManager() {
    // Limpar todas as texturas carregadas
    for (auto const& [path, texture] : m_textures) {
        delete texture;
    }
    m_textures.clear();

    // Limpar todas as spritesheets carregadas
    for (auto const& [path, spritesheet] : m_spritesheets) {
        delete spritesheet;
    }
    m_spritesheets.clear();

    std::cout << "ResourceManager destru�do, liberando assets." << std::endl;
}

Texture* ResourceManager::LoadTexture(const std::string& path) {
    // Primeiro, verificar se a textura j� foi carregada
    if (m_textures.find(path) != m_textures.end()) {
        std::cout << "Textura j� carregada: " << path << std::endl;
        return m_textures[path];
    }

    // Se n�o estiver carregada, carregar agora
    Texture* newTexture = new Texture(path.c_str());
    if (newTexture->getID() != 0) {
        m_textures[path] = newTexture;
        std::cout << "Textura carregada: " << path << std::endl;
        return newTexture;
    }
    else {
        std::cerr << "Falha ao carregar a textura: " << path << std::endl;
        delete newTexture; // Limpar a textura inv�lida
        return nullptr;
    }
}

Texture* ResourceManager::GetTexture(const std::string& path) const {
    auto it = m_textures.find(path);
    if (it != m_textures.end()) {
        return it->second;
    }
    std::cerr << "Textura n�o encontrada: " << path << std::endl;
    return nullptr;
}

Sprite* ResourceManager::LoadSprite(const std::string& texturePath) {
    Texture* texture = LoadTexture(texturePath);
    if (texture) {
        Sprite* newSprite = new Sprite(texture);
      //  std::cout << "Sprite carregado da textura: " + texturePath + " (UV: " + (minUV) + " - " + (maxUV) + ")" << std::endl;
        return newSprite;
    }
    else {
        std::cerr << "Falha ao carregar o sprite, pois a textura n�o p�de ser carregada: " << texturePath << std::endl;
        return nullptr;
    }
}

Spritesheet* ResourceManager::LoadSpritesheet(const std::string& texturePath, int spriteWidth, int spriteHeight) {
    Texture* texture = LoadTexture(texturePath);
    if (texture) {
        // Primeiro, verificar se a spritesheet j� foi carregada com esses par�metros
        std::string key = texturePath + "_" + std::to_string(spriteWidth) + "x" + std::to_string(spriteHeight);
        if (m_spritesheets.find(key) != m_spritesheets.end()) {
            std::cout << "Spritesheet j� carregada: " << key << std::endl;
            return m_spritesheets[key];
        }

        Spritesheet* newSpritesheet = new Spritesheet(texture->path, spriteWidth, spriteHeight);
        if (newSpritesheet->isValid()) {
            m_spritesheets[key] = newSpritesheet;
            std::cout << "Spritesheet carregada: " << key << std::endl;
            return newSpritesheet;
        }
        else {
            std::cerr << "Falha ao criar a spritesheet para: " << texturePath << std::endl;
            delete newSpritesheet;
            return nullptr;
        }
    }
    else {
        std::cerr << "Falha ao carregar a spritesheet, pois a textura n�o p�de ser carregada: " << texturePath << std::endl;
        return nullptr;
    }
}

Spritesheet* ResourceManager::GetSpritesheet(const std::string& path) const {
    auto it = m_spritesheets.find(path);
    if (it != m_spritesheets.end()) {
        return it->second;
    }
    std::cerr << "Spritesheet n�o encontrada: " << path << std::endl;
    return nullptr;
}