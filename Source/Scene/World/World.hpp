#ifndef WORLD_HPP
#define WORLD_HPP

#include <map>
#include <memory>

class Chunk;

class World {
public:
    World(int chunkWidth, int chunkHeight, int tilesPerChunkWidth, int tilesPerChunkHeight);
    ~World();

    std::shared_ptr<Chunk> getChunk(int chunkX, int chunkY);
    int getTile(int worldX, int worldY);
    void setTile(int worldX, int worldY, int tileId);

private:
    int m_chunkWidth; // Largura do grid de chunks
    int m_chunkHeight; // Altura do grid de chunks
    int m_tilesPerChunkWidth;
    int m_tilesPerChunkHeight;
    std::map<std::pair<int, int>, std::shared_ptr<Chunk>> m_chunks;

    std::pair<int, int> worldToChunkCoordinates(int worldX, int worldY) const;
    std::pair<int, int> worldToLocalChunkCoordinates(int worldX, int worldY) const;
};

#endif // WORLD_HPP