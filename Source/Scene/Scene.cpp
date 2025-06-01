#include "Scene.hpp"

void Scene::AddSceneObject(const SceneObject& object) {
    objects.push_back(object);
    std::cout << "Added SceneObject to scene '" << m_name << "'" << std::endl;
}

void Scene::Update(float deltaTime) {
    for (auto& object : objects) {
        object.Update(deltaTime); // Assumindo que SceneObject tem um m�todo Update
        // OU, se os componentes s�o gerenciados separadamente dentro de SceneObject:
        // for (auto& component : object.GetComponents()) {
        //     component->Update(deltaTime); // Assumindo que os componentes t�m um m�todo Update
        // }
    }
}