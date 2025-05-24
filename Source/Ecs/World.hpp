#ifndef LUNA_ECS_WORLD_HPP
#define LUNA_ECS_WORLD_HPP

#include <vector>
#include <memory>
#include <unordered_map>
#include <typeindex> // Incluir para std::type_index

#include "Entity.hpp"
#include "Transform.hpp"
#include "SpriteRenderer.hpp"

namespace Luna::Ecs {

    class World {
    public:
        Entity createEntity();
        void destroyEntity(Entity entity);

        template <typename T, typename... Args>
        T& addComponent(Entity entity, Args&&... args);

        template <typename T>
        T* getComponent(Entity entity) const; // ADICIONADO 'const'

        template <typename T>
        bool hasComponent(Entity entity) const;

        // Acesso público para iteração no RenderingSystem
        std::vector<Entity> entities; // Mantido público para iteração no sistema de renderização
        std::unordered_map<EntityID, std::unordered_map<std::type_index, std::shared_ptr<void>>> components;

    private:
        
    };

} // namespace Luna::Ecs

#endif // LUNA_ECS_WORLD_HPP