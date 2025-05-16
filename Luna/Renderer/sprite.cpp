#include "sprite.hpp"
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

Sprite::Sprite(const std::string& texturePath, const glm::vec3& position, const glm::vec2& size)
    : m_position(position), m_size(size) {
    m_texture.loadFromFile(texturePath);
}

Sprite::~Sprite() {
    // A textura é gerenciada pela classe Texture, então não precisamos deletar aqui.
}

void Sprite::draw(const Shader& shader) const {
    shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, m_position);
    model = glm::scale(model, glm::vec3(m_size, 1.0f));

    shader.setMat4("model", model);

    m_texture.bind(0);
    shader.setInt("texture1", 0);

    // Assumindo que o VAO já está bound externamente
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    m_texture.unbind();
}