#ifndef SPRITE_RENDERER_HPP
#define SPRITE_RENDERER_HPP

#include "Source/Ecs/Component.hpp"
#include "Source/Renderer/Texture.hpp" // Inclua Texture
#include <glm.hpp>                 // Para glm::vec4 (cor)
#include <memory>                      // Para std::shared_ptr

// Forward declarations para Renderer2D e SceneObject (se não inclusos em Component.hpp)
class Renderer2D;
class SceneObject; // Já forward-declarado em Component.hpp

class SpriteRenderer : public Component {
public:
    SpriteRenderer(std::shared_ptr<Texture> texture = nullptr, const glm::vec4& color = glm::vec4(1.0f));

    // O método Render agora acessará a Transform2D do owner_
    void Render(Renderer2D& renderer) override;
    void OnGui() override;

    void SetTexture(std::shared_ptr<Texture> texture);
    std::shared_ptr<Texture> GetTexture() const { return m_texture; }

    void SetColor(const glm::vec4& color) { m_color = color; }
    const glm::vec4& GetColor() const { return m_color; }

private:
    std::shared_ptr<Texture> m_texture;
    glm::vec4 m_color;
};

#endif // SPRITE_RENDERER_HPP
