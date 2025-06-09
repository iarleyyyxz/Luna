#ifndef SPRITESHEET_HPP
#define SPRITESHEET_HPP

#include <glm.hpp> // Para glm::vec2, glm::vec4
#include <string>
#include <vector>

#include "Texture.hpp" // Para usar a classe Texture
#include "Sprite.hpp"  // Para criar objetos Sprite a partir da spritesheet

class Spritesheet {
public:
    // Construtor: Carrega uma textura como spritesheet e define as dimens�es dos frames.
    // filename: Caminho para o arquivo da imagem da spritesheet.
    // frameWidth: Largura de cada frame individual na spritesheet em pixels.
    // frameHeight: Altura de cada frame individual na spritesheet em pixels.
    Spritesheet(const std::string& filename, int frameWidth, int frameHeight);

    // Destrutor
    ~Spritesheet();

    // Obt�m um sprite espec�fico da spritesheet pelo �ndice do frame.
    // O �ndice � calculado como (linha * numColunas) + coluna.
    // Retorna um ponteiro para um novo Sprite alocado dinamicamente.
    // � responsabilidade do chamador deletar este Sprite quando n�o for mais necess�rio.
    Sprite* getSprite(int frameIndex);

    // Obt�m um sprite espec�fico da spritesheet pelas coordenadas de linha e coluna.
    // Retorna um ponteiro para um novo Sprite alocado dinamicamente.
    // � responsabilidade do chamador deletar este Sprite quando n�o for mais necess�rio.
    Sprite* getSprite(int col, int row);

    // Retorna a textura subjacente da spritesheet.
    Texture* getTexture() const { return m_texture; }

    // Verifica se a spritesheet foi carregada com sucesso.
    bool isValid() const { return m_texture != nullptr && m_texture->getID() != 0; }

private:
    Texture* m_texture;     // A textura que cont�m a spritesheet
    int m_frameWidth;       // Largura de um �nico frame
    int m_frameHeight;      // Altura de um �nico frame
    int m_numCols;          // N�mero de colunas de frames na spritesheet
    int m_numRows;          // N�mero de linhas de frames na spritesheet
    int m_totalFrames;      // N�mero total de frames na spritesheet
};

#endif // SPRITESHEET_HPP