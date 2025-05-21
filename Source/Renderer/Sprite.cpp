#include "Sprite.hpp"
#include <iostream>

// Construtor para um sprite que usa a textura inteira
Sprite::Sprite(Texture* texture) : m_texture(texture) {
    if (m_texture) {
        // Usa a textura inteira, ent�o UVs v�o de (0,0) a (1,1)
        m_texCoordMin = glm::vec2(0.0f, 0.0f);
        m_texCoordMax = glm::vec2(1.0f, 1.0f);
    }
    else {
        std::cerr << "ERRO::SPRITE::CONSTRUTOR: Textura nula fornecida." << std::endl;
        m_texCoordMin = glm::vec2(0.0f, 0.0f);
        m_texCoordMax = glm::vec2(0.0f, 0.0f); // Valores inv�lidos para indicar erro
    }
}

// Construtor para um sprite que usa uma parte espec�fica de uma textura (sprite sheet)
Sprite::Sprite(Texture* texture, const glm::vec4& sourceRect) : m_texture(texture) {
    if (m_texture) {
        calculateTexCoords(sourceRect);
    }
    else {
        std::cerr << "ERRO::SPRITE::CONSTRUTOR: Textura nula fornecida." << std::endl;
        m_texCoordMin = glm::vec2(0.0f, 0.0f);
        m_texCoordMax = glm::vec2(0.0f, 0.0f); // Valores inv�lidos para indicar erro
    }
}

Sprite::~Sprite() {
    // A textura n�o � de propriedade do sprite, ent�o n�o a deletamos aqui.
    // Ela deve ser gerenciada pelo sistema de recursos ou pela classe que a carregou.
}

void Sprite::setSourceRect(const glm::vec4& sourceRect) {
    if (m_texture) {
        calculateTexCoords(sourceRect);
    }
    else {
        std::cerr << "ERRO::SPRITE::SET_SOURCE_RECT: Textura nula. N�o � poss�vel definir sourceRect." << std::endl;
    }
}

void Sprite::calculateTexCoords(const glm::vec4& sourceRect) {
    if (m_texture->width == 0 || m_texture->height == 0) {
        std::cerr << "ERRO::SPRITE::CALCULATE_TEX_COORDS: Textura com dimens�es zero." << std::endl;
        m_texCoordMin = glm::vec2(0.0f, 0.0f);
        m_texCoordMax = glm::vec2(0.0f, 0.0f);
        return;
    }

    // sourceRect: x, y, width, height em pixels
    // Converte coordenadas de pixel para coordenadas UV normalizadas (0.0 a 1.0)
    m_texCoordMin.x = sourceRect.x / (float)m_texture->width;
    m_texCoordMin.y = sourceRect.y / (float)m_texture->height;
    m_texCoordMax.x = (sourceRect.x + sourceRect.z) / (float)m_texture->width;
    m_texCoordMax.y = (sourceRect.y + sourceRect.w) / (float)m_texture->height;

    // Inverter a coordenada Y se a textura for carregada com stbi_set_flip_vertically_on_load(true)
    // OpenGL espera que o Y=0 esteja na parte inferior, enquanto as imagens geralmente t�m Y=0 no topo.
    // Se stbi_set_flip_vertically_on_load(true) for usado, a imagem j� est� invertida.
    // Se a sua conven��o de mundo 2D for Y para baixo (como a maioria dos jogos 2D),
    // ent�o a coordenada Y da textura tamb�m deve ser invertida para corresponder ao seu mundo.
    // Se o seu mundo 2D tem Y para cima, e a textura foi virada, ent�o est� correto.
    // Para um mundo 2D com Y para baixo (origem superior esquerda), e stbi_set_flip_vertically_on_load(true):
    // A textura � virada na carga, ent�o o (0,0) da textura (canto superior esquerdo da imagem)
    // corresponde ao (0,1) das UVs no OpenGL.
    // Para corrigir isso e ter (0,0) da imagem = (0,0) UV (canto superior esquerdo),
    // precisamos inverter as coordenadas Y aqui.
    float tempY = m_texCoordMin.y;
    m_texCoordMin.y = m_texCoordMax.y;
    m_texCoordMax.y = tempY;

    // Nota: A invers�o de Y aqui � uma conven��o comum para jogos 2D onde a origem do mundo � superior esquerda
    // e as texturas s�o carregadas com stbi_set_flip_vertically_on_load(true).
    // Se a sua conven��o de coordenadas for diferente, ajuste conforme necess�rio.
}