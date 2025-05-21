#include "Spritesheet.hpp"
#include <iostream>

Spritesheet::Spritesheet(const std::string& filename, int frameWidth, int frameHeight)
    : m_texture(nullptr), m_frameWidth(frameWidth), m_frameHeight(frameHeight),
    m_numCols(0), m_numRows(0), m_totalFrames(0)
{
    // Carrega a textura da spritesheet
    m_texture = new Texture(filename.c_str());
    if (!m_texture->getID()) {
        std::cerr << "ERRO::SPRITESHEET::CONSTRUTOR: Falha ao carregar a textura: " << filename << std::endl;
        delete m_texture;
        m_texture = nullptr;
        return;
    }

    // Calcula o número de colunas e linhas de frames
    if (m_texture->width > 0 && m_texture->height > 0) {
        m_numCols = m_texture->width / m_frameWidth;
        m_numRows = m_texture->height / m_frameHeight;
        m_totalFrames = m_numCols * m_numRows;
        std::cout << "Spritesheet '" << filename << "' carregada. Dimensões: "
            << m_texture->width << "x" << m_texture->height
            << ", Frames: " << m_numCols << "x" << m_numRows
            << ", Total de frames: " << m_totalFrames << std::endl;
    }
    else {
        std::cerr << "ERRO::SPRITESHEET::CONSTRUTOR: Textura carregada com dimensões inválidas (0x0)." << std::endl;
        delete m_texture;
        m_texture = nullptr;
    }
}

Spritesheet::~Spritesheet() {
    // A textura é de propriedade da Spritesheet, então a deletamos aqui.
    if (m_texture) {
        delete m_texture;
        m_texture = nullptr;
    }
}

Sprite* Spritesheet::getSprite(int frameIndex) {
    if (!isValid()) {
        std::cerr << "ERRO::SPRITESHEET::GET_SPRITE: Spritesheet inválida." << std::endl;
        return nullptr;
    }
    if (frameIndex < 0 || frameIndex >= m_totalFrames) {
        std::cerr << "ERRO::SPRITESHEET::GET_SPRITE: Índice de frame fora dos limites: " << frameIndex << std::endl;
        return nullptr;
    }

    // Calcula a coluna e linha do frame
    int col = frameIndex % m_numCols;
    int row = frameIndex / m_numCols;

    return getSprite(col, row);
}

Sprite* Spritesheet::getSprite(int col, int row) {
    if (!isValid()) {
        std::cerr << "ERRO::SPRITESHEET::GET_SPRITE: Spritesheet inválida." << std::endl;
        return nullptr;
    }
    if (col < 0 || col >= m_numCols || row < 0 || row >= m_numRows) {
        std::cerr << "ERRO::SPRITESHEET::GET_SPRITE: Coordenadas de coluna/linha fora dos limites: ("
            << col << ", " << row << ")" << std::endl;
        return nullptr;
    }

    // Calcula as coordenadas de pixel do canto superior esquerdo do frame
    float x = (float)col * m_frameWidth;
    float y = (float)row * m_frameHeight;

    // Define o retângulo de origem para o Sprite
    glm::vec4 sourceRect = glm::vec4(x, y, (float)m_frameWidth, (float)m_frameHeight);

    // Cria e retorna um novo Sprite
    return new Sprite(m_texture, sourceRect);
}
