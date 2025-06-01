#include "World.hpp"
#include "Chunk.hpp"
#include <iostream>

World::World(int chunkWidth, int chunkHeight, int tilesPerChunkWidth, int tilesPerChunkHeight) :
    m_chunkWidth(chunkWidth), m_chunkHeight(chunkHeight),
    m_tilesPerChunkWidth(tilesPerChunkWidth), m_tilesPerChunkHeight(tilesPerChunkHeight)
{
    std::cout << "Mundo criado com " << chunkWidth << "x" << chunkHeight
        << " chunks, cada um com " << tilesPerChunkWidth << "x" << tilesPerChunkHeight << " tiles." << std::endl;
}

World::~World() {
    std::cout << "Mundo destruído, liberando chunks." << std::endl;
    m_chunks.clear(); // Shared pointers cuidam da desalocação dos Chunks
}

std::shared_ptr<Chunk> World::getChunk(int chunkX, int chunkY) {
    std::pair<int, int> key = { chunkX, chunkY };
    if (m_chunks.find(key) == m_chunks.end()) {
        auto newChunk = std::make_shared<Chunk>(m_tilesPerChunkWidth, m_tilesPerChunkHeight, chunkX, chunkY);
        m_chunks[key] = newChunk;
        return newChunk;
    }
    return m_chunks[key];
}

int World::getTile(int worldX, int worldY) {
    std::pair<int, int> chunkCoords = worldToChunkCoordinates(worldX, worldY);
    std::pair<int, int> localCoords = worldToLocalChunkCoordinates(worldX, worldY);

    std::shared_ptr<Chunk> chunk = getChunk(chunkCoords.first, chunkCoords.second);
    if (chunk) {
        return chunk->getTile(localCoords.first, localCoords.second);
    }
    return -1;
}

void World::setTile(int worldX, int worldY, int tileId) {
    std::pair<int, int> chunkCoords = worldToChunkCoordinates(worldX, worldY);
    std::pair<int, int> localCoords = worldToLocalChunkCoordinates(worldX, worldY);

    std::shared_ptr<Chunk> chunk = getChunk(chunkCoords.first, chunkCoords.second);
    if (chunk) {
        chunk->setTile(localCoords.first, localCoords.second, tileId);
    }
}

std::pair<int, int> World::worldToChunkCoordinates(int worldX, int worldY) const {
    int chunkX = worldX / m_tilesPerChunkWidth;
    if (worldX < 0 && worldX % m_tilesPerChunkWidth != 0) {
        chunkX--;
    }
    int chunkY = worldY / m_tilesPerChunkHeight;
    if (worldY < 0 && worldY % m_tilesPerChunkHeight != 0) {
        chunkY--;
    }
    return { chunkX, chunkY };
}

std::pair<int, int> World::worldToLocalChunkCoordinates(int worldX, int worldY) const {
    int localX = worldX % m_tilesPerChunkWidth;
    if (localX < 0) {
        localX += m_tilesPerChunkWidth;
    }
    int localY = worldY % m_tilesPerChunkHeight;
    if (localY < 0) {
        localY += m_tilesPerChunkHeight;
    }
    return { localX, localY };
}