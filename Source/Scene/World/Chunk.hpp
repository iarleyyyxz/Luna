#ifndef CHUNK_HPP
#define CHUNK_HPP

#include <vector>
#include <memory>

class Chunk {
public:
    Chunk(int width, int height, int x, int y);
    ~Chunk();

    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    int getX() const { return m_x; }
    int getY() const { return m_y; }

    int getTile(int x, int y) const;
    void setTile(int x, int y, int tileId);

private:
    int m_width;
    int m_height;
    int m_x; // Posição do chunk no grid do mundo
    int m_y;
    std::vector<int> m_tiles; // IDs dos tiles
};

#endif // CHUNK_HPP