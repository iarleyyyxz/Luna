#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <glm.hpp> // Para glm::vec2
#include "Renderer/Texture.hpp" // Para usar a classe Texture

class Sprite {
public:
    // Construtor para um sprite que usa a textura inteira
    Sprite(Texture* texture);

    // Construtor para um sprite que usa uma parte espec�fica de uma textura (sprite sheet)
    // sourceRect: glm::vec4(x, y, width, height) em pixels da textura original
    Sprite(Texture* texture, const glm::vec4& sourceRect);

    ~Sprite();

    // Obt�m a textura do sprite
    Texture* getTexture() const { return m_texture; }
    void SetTexture(Texture* texture) { this->m_texture = texture; }

    // Obt�m as coordenadas de textura (UVs) para o sprite
    // Estas s�o normalizadas (0.0 a 1.0)
    glm::vec2 getTexCoordMin() const { return m_texCoordMin; }
    glm::vec2 getTexCoordMax() const { return m_texCoordMax; }

    // Define as coordenadas de textura (UVs) para o sprite
    // �til para anima��es ou mudar a parte vis�vel da textura
    void setSourceRect(const glm::vec4& sourceRect);

private:
    Texture* m_texture;       // Ponteiro para a textura que o sprite usa
    glm::vec2 m_texCoordMin;  // Coordenadas de textura m�nimas (u, v)
    glm::vec2 m_texCoordMax;  // Coordenadas de textura m�ximas (u, v)

    // Fun��o auxiliar para calcular as coordenadas de textura normalizadas
    void calculateTexCoords(const glm::vec4& sourceRect);
};

#endif // SPRITE_HPP
