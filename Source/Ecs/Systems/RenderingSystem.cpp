// RenderingSystem.cpp
#include "RenderingSystem.hpp" // Inclua o header correspondente
#include "Renderer2D.hpp"
#include "Source/Ecs/World.hpp"

namespace Luna::Ecs::Systems {

    RenderingSystem::RenderingSystem(World& world, Renderer2D& renderer, float screenWidth, float screenHeight)
        // Inicialize os membros da sua classe aqui, se necessário
        : m_world(world), m_renderer(renderer), m_screenWidth(screenWidth), m_screenHeight(screenHeight) {
        // ... qualquer outra inicialização ...
    }

    void RenderingSystem::Update() {
        for (const auto& entity : m_world.entities) {
            Ecs::Transform* transform = m_world.getComponent<Ecs::Transform>(entity);
            Ecs::SpriteRenderer* sprite = m_world.getComponent<Ecs::SpriteRenderer>(entity);

            if (transform && sprite) {
                m_renderer.drawQuad(transform->position, transform->scale, sprite->color);
            }
        }
    }

} // namespace Luna::Ecs::Systems