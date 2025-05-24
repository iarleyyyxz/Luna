// Scene/Scene.hpp
#ifndef LUNA_SCENE_SCENE_HPP
#define LUNA_SCENE_SCENE_HPP

#include "Source/Ecs/World.hpp"
#include "Source/Ecs/Systems/RenderingSystem.hpp"
#include "Source/Renderer/Renderer2D.hpp"
#include <fstream>
#include <nlohmann/json.hpp> // Incluir a biblioteca JSON

namespace Luna {

    using json = nlohmann::json;

    class Scene {
    public:
        Scene(float screenWidth, float screenHeight, Renderer2D& renderer);
        ~Scene();

        Luna::Ecs::Entity CreateEntity();
        void DestroyEntity(Luna::Ecs::Entity entity);

        template <typename T, typename... Args>
        T& AddComponent(Luna::Ecs::Entity entity, Args&&... args);

        template <typename T>
        T* GetComponent(Luna::Ecs::Entity entity) const;

        template <typename T>
        bool HasComponent(Luna::Ecs::Entity entity) const;

        void Update(float deltaTime);
        void Render();

        void Serialize(const std::string& filepath) const;
        bool Deserialize(const std::string& filepath);

        Luna::Ecs::World m_world;
        Luna::Ecs::Systems::RenderingSystem m_renderingSystem;

        std::string name;

    private:
        
        Renderer2D& m_renderer;
        float m_screenWidth;
        float m_screenHeight;

        friend void to_json(json& j, const Ecs::Transform& p);
        friend void from_json(const json& j, Ecs::Transform& p);
        friend void to_json(json& j, const Ecs::SpriteRenderer& p);
        friend void from_json(const json& j, Ecs::SpriteRenderer& p);
    };

} // namespace Luna

#endif // LUNA_SCENE_SCENE_HPP