#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <string>
#include <map>
#include <glm.hpp>
#include "Texture.hpp"
#include "Sprite.hpp"
#include "Spritesheet.hpp" // Incluímos Spritesheet

class ResourceManager {
public:
    ResourceManager();
    ~ResourceManager();

    // Carrega uma textura do arquivo especificado.
    // Se a textura já estiver carregada, retorna a existente.
    Texture* LoadTexture(const std::string& path);

    // Obtém uma textura já carregada pelo seu caminho.
    Texture* GetTexture(const std::string& path) const;

    // Carrega um sprite, carregando a textura se necessário.
    Sprite* LoadSprite(const std::string& texturePath);

    // Carrega uma spritesheet, carregando a textura subjacente se necessário.
    Spritesheet* LoadSpritesheet(const std::string& texturePath, int spriteWidth, int spriteHeight);

    // Obtém uma spritesheet já carregada (se decidirmos armazená-las).
    Spritesheet* GetSpritesheet(const std::string& path) const;

private:
    std::map<std::string, Texture*> m_textures;
    std::map<std::string, Spritesheet*> m_spritesheets; // Mapa para armazenar spritesheets carregadas
};

#endif // RESOURCE_MANAGER_HPP