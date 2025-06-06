#include "Source/Ecs/SpriteRenderer.hpp"
#include "Source/Ecs/SceneObject.hpp"   // Para SceneObject
#include "Source/Ecs/Transform2D.hpp"   // Para Transform2D
#include "Source/Renderer/Renderer2D.hpp" // Para Renderer2D
#include <iostream>
#include <imgui.h>

SpriteRenderer::SpriteRenderer(std::shared_ptr<Texture> texture, const glm::vec4& color)
    : Component(), m_texture(texture), m_color(color) {
    // Constructor. Owner is set by SceneObject::AddComponent.
}

void SpriteRenderer::Render(Renderer2D& renderer) {
    std::shared_ptr<SceneObject> owner = GetOwner();
    if (!owner) {
        std::cerr << "ERRO::SPRITERENDERER::RENDER: Owner nao definido." << std::endl;
        return;
    }

    // Obtenha a referencia direta para o Transform2D do owner
    Transform2D& transform = owner->GetTransform();

    // Verifique se a textura está definida
    if (!m_texture) {
        renderer.drawQuad(transform.position, glm::vec2(transform.scale.x, transform.scale.y), m_color);
        return;
    }

    // Chame o drawQuad do Renderer2D com as propriedades do Transform2D
    // Nota: Se o seu Renderer2D precisar de rotação, você precisará adaptar drawQuad.
    // Esta chamada assume que drawQuad usa posição, tamanho (escala) e cor/textura.
    renderer.drawQuad(
        transform.position,
        glm::vec2(transform.scale.x, transform.scale.y),
        *m_texture,
        glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f), // Coordenadas UV padrão
        m_color
    );
}

void SpriteRenderer::OnGui() {
    if (ImGui::TreeNodeEx("Sprite Renderer", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::ColorEdit4("Color", &m_color.r);
        if (m_texture) {
            ImGui::Text("Texture: %s (ID: %d)", m_texture->path.c_str(), m_texture->ID);
        }
        else {
            ImGui::Text("Texture: None");
        }
        ImGui::TreePop();
    }
}

void SpriteRenderer::SetTexture(std::shared_ptr<Texture> texture) {
    m_texture = texture;
}
