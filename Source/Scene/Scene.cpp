#include "Scene.hpp"

void Scene::AddSceneObject(const SceneObject& object) {
    objects.push_back(object);
    std::cout << "Added SceneObject to scene '" << m_name << "'" << std::endl;
}