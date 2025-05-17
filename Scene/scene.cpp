#include "Scene.hpp"
#include "Component/gameObject.hpp"

namespace Scene {

    Scene::Scene() {}

    Scene::~Scene() {
        for (Component::GameObject* gameObject : gameObjects) {
            delete gameObject;
        }
        gameObjects.clear();
    }

    void Scene::addGameObject(Component::GameObject* gameObject) {
        gameObjects.push_back(gameObject);
    }

    void Scene::update(float deltaTime) {
        for (Component::GameObject* gameObject : gameObjects) {
            gameObject->update(deltaTime);
        }
    }

    void Scene::render() {
        for (Component::GameObject* gameObject : gameObjects) {
            gameObject->render();
        }
    }

} // namespace Scene