#ifndef LUNA_ECS_SYSTEMS_RENDERING_SYSTEM_HPP
#define LUNA_ECS_SYSTEMS_RENDERING_SYSTEM_HPP

#include "Source/Renderer/Renderer2D.hpp"
#include "Source/Ecs/World.hpp"
#include <glm.hpp>

namespace Luna::Ecs::Systems {

    class RenderingSystem {
    public:
        RenderingSystem(World& world, Renderer2D& renderer, float screenWidth, float screenHeight);
        void Update();

        World& m_world;
        Renderer2D& m_renderer;
    private:
       
        float m_screenWidth;
        float m_screenHeight;
    };

} // namespace Luna::Ecs::Systems

#endif // LUNA_ECS_SYSTEMS_RENDERING_SYSTEM_HPP