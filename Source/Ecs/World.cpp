// World.cpp
#include "World.hpp"
#include <typeindex>
#include <iostream>
#include <algorithm> // Para std::remove_if

namespace Luna::Ecs {

    Entity World::createEntity() {
        Entity entity;
        entities.push_back(entity);
        return entity;
    }

    void World::destroyEntity(Entity entity) {
        entities.erase(std::remove_if(entities.begin(), entities.end(),
            [&](const Entity& e) { return e.getID() == entity.getID(); }),
            entities.end());
        components.erase(entity.getID());
    }

    template <typename T, typename... Args>
    T& World::addComponent(Entity entity, Args&&... args) {
        std::type_index typeIndex = typeid(T);
        EntityID entityID = entity.getID();

        if (components.find(entityID) == components.end()) {
            components[entityID] = std::unordered_map<std::type_index, std::shared_ptr<void>>();
        }

        // Usa std::make_shared porque 'components' armazena shared_ptr
        components[entityID][typeIndex] = std::make_shared<T>(std::forward<Args>(args)...);
        return *static_cast<T*>(components[entityID][typeIndex].get());
    }

    template <typename T>
    T* World::getComponent(Entity entity) const {
        std::type_index typeIndex = typeid(T);
        EntityID entityID = entity.getID();

        if (components.count(entityID) && components.at(entityID).count(typeIndex)) {
            return static_cast<T*>(components.at(entityID).at(typeIndex).get());
        }
        return nullptr;
    }

    template <typename T>
    bool World::hasComponent(Entity entity) const {
        std::type_index typeIndex = typeid(T);
        EntityID entityID = entity.getID();

        return components.count(entityID) && components.at(entityID).count(typeIndex);
    }

    // Explicit template instantiations

    // Instanciações para addComponent que aceitam referências a objetos de componente (usado em Deserialize)
    template Luna::Ecs::Transform& World::addComponent<Luna::Ecs::Transform>(Luna::Ecs::Entity, Luna::Ecs::Transform&);
    template Luna::Ecs::SpriteRenderer& World::addComponent<Luna::Ecs::SpriteRenderer>(Luna::Ecs::Entity, Luna::Ecs::SpriteRenderer&);

    // Instanciações para addComponent que aceitam rvalue references (usado com std::move ou temporários)
    template Luna::Ecs::Transform& World::addComponent<Luna::Ecs::Transform>(Luna::Ecs::Entity, Luna::Ecs::Transform&&);
    template Luna::Ecs::SpriteRenderer& World::addComponent<Luna::Ecs::SpriteRenderer>(Luna::Ecs::Entity, Luna::Ecs::SpriteRenderer&&);

    // Instanciações para addComponent que aceitam os argumentos do construtor (glm::vec3, string, vec4)
    template Luna::Ecs::Transform& World::addComponent<Luna::Ecs::Transform>(Luna::Ecs::Entity, glm::vec3&&, glm::vec3&&, glm::vec3&&);
    template Luna::Ecs::SpriteRenderer& World::addComponent<Luna::Ecs::SpriteRenderer>(Luna::Ecs::Entity, const std::string&, const glm::vec4&);

    // Instanciações para getComponent
    template Luna::Ecs::Transform* World::getComponent<Luna::Ecs::Transform>(Luna::Ecs::Entity) const;
    template Luna::Ecs::SpriteRenderer* World::getComponent<Luna::Ecs::SpriteRenderer>(Luna::Ecs::Entity) const;

    // Instanciações para hasComponent
    template bool Luna::Ecs::World::hasComponent<Luna::Ecs::Transform>(Luna::Ecs::Entity) const;
    template bool Luna::Ecs::World::hasComponent<Luna::Ecs::SpriteRenderer>(Luna::Ecs::Entity) const;

} // namespace Luna::Ecs