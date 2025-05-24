// Scene.cpp
#include "Scene.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <nlohmann/json.hpp>

namespace Luna {

    void to_json(json& j, const Ecs::Transform& p) {
        j = json{ {"position", {p.position.x, p.position.y, p.position.z}},
                  {"rotation", {p.rotation.x, p.rotation.y, p.rotation.z}},
                  {"scale", {p.scale.x, p.scale.y, p.scale.z}} };
    }

    void from_json(const json& j, Ecs::Transform& p) {
        auto position = j.at("position").get<std::vector<float>>();
        p.position.x = position[0];
        p.position.y = position[1];
        p.position.z = position[2];
        auto rotation = j.at("rotation").get<std::vector<float>>();
        p.rotation.x = rotation[0];
        p.rotation.y = rotation[1];
        p.rotation.z = rotation[2];
        auto scale = j.at("scale").get<std::vector<float>>();
        p.scale.x = scale[0];
        p.scale.y = scale[1];
        p.scale.z = scale[2];
    }

    void to_json(json& j, const Ecs::SpriteRenderer& p) {
        j = json{ {"texturePath", p.texturePath},
                  {"color", {p.color.r, p.color.g, p.color.b, p.color.a}} };
    }

    void from_json(const json& j, Ecs::SpriteRenderer& p) {
        p.texturePath = j.at("texturePath").get<std::string>();
        auto color = j.at("color").get<std::vector<float>>();
        p.color.r = color[0];
        p.color.g = color[1];
        p.color.b = color[2];
        p.color.a = color[3];
    }

    Scene::Scene(float screenWidth, float screenHeight, Renderer2D& renderer)
        : m_renderingSystem(m_world, renderer, screenWidth, screenHeight),
        m_renderer(renderer),
        m_screenWidth(screenWidth),
        m_screenHeight(screenHeight) {
        std::cout << "Scene criada." << std::endl;
    }

    Scene::~Scene() {
        std::cout << "Scene destruída." << std::endl;
    }

    Luna::Ecs::Entity Scene::CreateEntity() {
        return m_world.createEntity();
    }

    void Scene::DestroyEntity(Luna::Ecs::Entity entity) {
        m_world.destroyEntity(entity);
    }

    template <typename T, typename... Args>
    T& Scene::AddComponent(Luna::Ecs::Entity entity, Args&&... args) {
        return m_world.addComponent<T>(entity, std::forward<Args>(args)...);
    }

    template <typename T>
    T* Scene::GetComponent(Luna::Ecs::Entity entity) const { // Adicionado 'const' aqui
        return m_world.getComponent<T>(entity);
    }

    template <typename T>
    bool Scene::HasComponent(Luna::Ecs::Entity entity) const {
        return m_world.hasComponent<T>(entity);
    }

    void Scene::Update(float deltaTime) {
        // Lógica de update da cena, se necessário
    }

    void Scene::Render() {
        m_renderingSystem.Update(); // Corrigido para 'update()'
    }

    void Scene::Serialize(const std::string& filepath) const {
        json data;
        data["entities"] = json::array();
        for (const auto& entity : m_world.entities) {
            json entityData;
            entityData["id"] = entity.getID();
            json componentsData = json::object();

            if (m_world.hasComponent<Ecs::Transform>(entity)) {
                json transformJson;
                to_json(transformJson, *m_world.getComponent<Ecs::Transform>(entity));
                componentsData["Transform"] = transformJson;
            }

            if (m_world.hasComponent<Ecs::SpriteRenderer>(entity)) {
                json spriteRendererJson; // Nome da variável corrigido
                to_json(spriteRendererJson, *m_world.getComponent<Ecs::SpriteRenderer>(entity));
                componentsData["SpriteRenderer"] = spriteRendererJson;
            }

            entityData["components"] = componentsData;
            data["entities"].push_back(entityData);
        }

        std::ofstream file(filepath);
        if (file.is_open()) {
            file << std::setw(4) << data << std::endl;
            file.close();
            std::cout << "Cena serializada para: " << filepath << std::endl;
        }
        else {
            std::cerr << "Não foi possível abrir o arquivo para serialização: " << filepath << std::endl;
        }
    }

    bool Scene::Deserialize(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Não foi possível abrir o arquivo para deserialização: " << filepath << std::endl;
            return false;
        }

        json data;
        file >> data;
        file.close();

        m_world.entities.clear();
        m_world.components.clear();

        for (const auto& entityData : data["entities"]) {
            Ecs::EntityID id = entityData["id"].get<Ecs::EntityID>();
            Ecs::Entity entity(id);
            m_world.entities.push_back(entity);

            auto componentsData = entityData["components"];

            if (componentsData.contains("Transform")) {
                Ecs::Transform transform;
                from_json(componentsData["Transform"], transform);
                m_world.addComponent<Ecs::Transform>(entity, transform); // Passando por referência (lvalue)
            }

            if (componentsData.contains("SpriteRenderer")) {
                Ecs::SpriteRenderer sprRenderer;
                from_json(componentsData["SpriteRenderer"], sprRenderer);
                m_world.addComponent<Ecs::SpriteRenderer>(entity, sprRenderer); // Passando por referência (lvalue)
            }
        }

        std::cout << "Cena desserializada de: " << filepath << std::endl;
        return true;
    }

    // Explicit template instantiations for Scene methods
    template Luna::Ecs::Transform& Luna::Scene::AddComponent<Luna::Ecs::Transform>(Luna::Ecs::Entity, glm::vec3&&, glm::vec3&&, glm::vec3&&);
    template Luna::Ecs::SpriteRenderer& Luna::Scene::AddComponent<Luna::Ecs::SpriteRenderer>(Luna::Ecs::Entity, const std::string&, const glm::vec4&);

    template Luna::Ecs::Transform* Luna::Scene::GetComponent<Luna::Ecs::Transform>(Luna::Ecs::Entity) const;
    template Luna::Ecs::SpriteRenderer* Luna::Scene::GetComponent<Luna::Ecs::SpriteRenderer>(Luna::Ecs::Entity) const;

    template bool Luna::Scene::HasComponent<Luna::Ecs::Transform>(Luna::Ecs::Entity) const;
    template bool Luna::Scene::HasComponent<Luna::Ecs::SpriteRenderer>(Luna::Ecs::Entity) const;

} // namespace Luna