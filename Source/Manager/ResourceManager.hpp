#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <string>
#include <map>
#include <glm.hpp>
#include "Texture.hpp"
#include "Sprite.hpp"
#include "Spritesheet.hpp" // Inclu�mos Spritesheet

class ResourceManager {
public:
    ResourceManager();
    ~ResourceManager();

    // Carrega uma textura do arquivo especificado.
    // Se a textura j� estiver carregada, retorna a existente.
    Texture* LoadTexture(const std::string& path);

    // Obt�m uma textura j� carregada pelo seu caminho.
    Texture* GetTexture(const std::string& path) const;

    // Carrega um sprite, carregando a textura se necess�rio.
    Sprite* LoadSprite(const std::string& texturePath);

    // Carrega uma spritesheet, carregando a textura subjacente se necess�rio.
    Spritesheet* LoadSpritesheet(const std::string& texturePath, int spriteWidth, int spriteHeight);

    // Obt�m uma spritesheet j� carregada (se decidirmos armazen�-las).
    Spritesheet* GetSpritesheet(const std::string& path) const;

private:
    std::map<std::string, Texture*> m_textures;
    std::map<std::string, Spritesheet*> m_spritesheets; // Mapa para armazenar spritesheets carregadas
};

#endif // RESOURCE_MANAGER_HPP