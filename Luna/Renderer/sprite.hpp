#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "shader.hpp"
#include "texture.hpp"
#include <string>
#include <glm.hpp>

class Sprite {
public:
    Sprite(const std::string& texturePath, const glm::vec3& position = glm::vec3(0.0f), const glm::vec2& size = glm::vec2(1.0f));
    ~Sprite();

    void draw(const Shader& shader) const;

    Texture getTexture() { 
        return m_texture; 
    }

private:
    Texture m_texture;
    glm::vec3 m_position;
    glm::vec2 m_size;

    // VAO, VBO para cada sprite instance? Ou gerenciado externamente?
    // Por simplicidade inicial, vamos assumir que o VAO/VBO é configurado externamente
    // e o Sprite apenas se preocupa com a textura e a transformação.
};

#endif // SPRITE_HPP