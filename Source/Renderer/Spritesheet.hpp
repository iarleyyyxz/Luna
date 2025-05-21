#ifndef SPRITESHEET_HPP
#define SPRITESHEET_HPP

#include <glm.hpp> // Para glm::vec2, glm::vec4
#include <string>
#include <vector>

#include "Texture.hpp" // Para usar a classe Texture
#include "Sprite.hpp"  // Para criar objetos Sprite a partir da spritesheet

class Spritesheet {
public:
    // Construtor: Carrega uma textura como spritesheet e define as dimensões dos frames.
    // filename: Caminho para o arquivo da imagem da spritesheet.
    // frameWidth: Largura de cada frame individual na spritesheet em pixels.
    // frameHeight: Altura de cada frame individual na spritesheet em pixels.
    Spritesheet(const std::string& filename, int frameWidth, int frameHeight);

    // Destrutor
    ~Spritesheet();

    // Obtém um sprite específico da spritesheet pelo índice do frame.
    // O índice é calculado como (linha * numColunas) + coluna.
    // Retorna um ponteiro para um novo Sprite alocado dinamicamente.
    // É responsabilidade do chamador deletar este Sprite quando não for mais necessário.
    Sprite* getSprite(int frameIndex);

    // Obtém um sprite específico da spritesheet pelas coordenadas de linha e coluna.
    // Retorna um ponteiro para um novo Sprite alocado dinamicamente.
    // É responsabilidade do chamador deletar este Sprite quando não for mais necessário.
    Sprite* getSprite(int col, int row);

    // Retorna a textura subjacente da spritesheet.
    Texture* getTexture() const { return m_texture; }

    // Verifica se a spritesheet foi carregada com sucesso.
    bool isValid() const { return m_texture != nullptr && m_texture->getID() != 0; }

private:
    Texture* m_texture;     // A textura que contém a spritesheet
    int m_frameWidth;       // Largura de um único frame
    int m_frameHeight;      // Altura de um único frame
    int m_numCols;          // Número de colunas de frames na spritesheet
    int m_numRows;          // Número de linhas de frames na spritesheet
    int m_totalFrames;      // Número total de frames na spritesheet
};

#endif // SPRITESHEET_HPP