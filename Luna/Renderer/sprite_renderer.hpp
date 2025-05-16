#ifndef SPRITE_RENDERER_HPP
#define SPRITE_RENDERER_HPP

#include "shader.hpp"
#include "texture.hpp"
#include "sprite_renderer.hpp" // Para a struct Vertex
#include <vector>
#include <glm.hpp>

class SpriteRenderer {
public:
    SpriteRenderer(const Shader& shader);
    ~SpriteRenderer();

    void beginBatch();
    void drawSprite(const Texture& texture, const glm::vec3& position, const glm::vec2& size, float rotation = 0.0f, const glm::vec4& color = glm::vec4(1.0f));
    void endBatch();
    void renderBatch();

private:
    Shader m_shader;
    unsigned int m_VAO, m_VBO;
    std::vector<Vertex> m_vertices;
    unsigned int m_maxQuads = 1000; // Um número razoável para começar
    unsigned int m_quadCount = 0;

    void initRenderData();
};

struct Vertex {
    glm::vec3 position;
    glm::vec2 texCoords;
};

#endif // SPRITE_RENDERER_HPP