#include "Chunk.hpp"
#include <iostream>

Chunk::Chunk(int width, int height, int x, int y) :
    m_width(width), m_height(height), m_x(x), m_y(y), m_tiles(width* height, 0) // Inicializa com 0 (vazio)
{
    std::cout << "Chunk criado em (" << x << ", " << y << ") com " << width << "x" << height << " tiles." << std::endl;
}

Chunk::~Chunk() {
    std::cout << "Chunk em (" << m_x << ", " << m_y << ") destruído." << std::endl;
}

int Chunk::getTile(int x, int y) const {
    if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
        return m_tiles[y * m_width + x];
    }
    return -1; // Fora dos limites
}

void Chunk::setTile(int x, int y, int tileId) {
    if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
        m_tiles[y * m_width + x] = tileId;
    }
}