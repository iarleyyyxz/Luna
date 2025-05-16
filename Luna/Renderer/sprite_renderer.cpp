#include "sprite_renderer.hpp"
#include <GL/glew.h>
#include <iostream>
#include <gtc/matrix_transform.hpp>

SpriteRenderer::SpriteRenderer(const Shader& shader) : m_shader(shader) {
    initRenderData();
}

SpriteRenderer::~SpriteRenderer() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
}

void SpriteRenderer::initRenderData() {
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_maxQuads * 4 * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

    // Atributos do vértice
    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    // texCoords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void SpriteRenderer::beginBatch() {
    m_vertices.clear();
    m_quadCount = 0;
}

void SpriteRenderer::drawSprite(const Texture& texture, const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color) {
    // Preparar os dados dos 4 vértices do quad
    float x = position.x;
    float y = position.y;
    float z = position.z;
    float w = size.x;
    float h = size.y;

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f));

    // Adicionar os vértices ao batch
    // Triângulo 1
    m_vertices.push_back({ {model * glm::vec4(-0.5f,  0.5f, z, 1.0f)}, {0.0f, 1.0f} }); // Top-left
    m_vertices.push_back({ {model * glm::vec4(0.5f, -0.5f, z, 1.0f)}, {1.0f, 0.0f} }); // Bottom-right
    m_vertices.push_back({ {model * glm::vec4(-0.5f, -0.5f, z, 1.0f)}, {0.0f, 0.0f} }); // Bottom-left
    // Triângulo 2
    m_vertices.push_back({ {model * glm::vec4(-0.5f,  0.5f, z, 1.0f)}, {0.0f, 1.0f} }); // Top-left
    m_vertices.push_back({ {model * glm::vec4(0.5f,  0.5f, z, 1.0f)}, {1.0f, 1.0f} }); // Top-right
    m_vertices.push_back({ {model * glm::vec4(0.5f, -0.5f, z, 1.0f)}, {1.0f, 0.0f} }); // Bottom-right

    m_quadCount++;
}

void SpriteRenderer::endBatch() {
    // Não precisamos fazer nada aqui por enquanto, a renderização acontece em renderBatch
}

void SpriteRenderer::renderBatch() {
    if (m_vertices.empty()) return;

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices.size() * sizeof(Vertex), m_vertices.data());

    m_shader.use();
    // Bind da textura (por enquanto, assumimos que todos os sprites usam a mesma textura no batch)
    // No futuro, precisaremos lidar com múltiplas texturas
    glActiveTexture(GL_TEXTURE0);
    // Aqui precisamos de uma forma de saber qual textura usar para o batch
    // Por agora, vamos assumir que a última textura desenhada é a que queremos usar
    // Isso precisará ser melhorado para batches multi-textura
    // texture.bind(); // Não temos acesso direto à textura aqui

    glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());

    glBindVertexArray(0);
}