#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include "Component/gameObject.hpp"
#include "Component/component.hpp"

namespace Scene {

    class Scene {
    private:
        std::vector<Component::GameObject*> gameObjects;

    public:
        Scene();
        ~Scene();

        void addGameObject(Component::GameObject* gameObject);
        void update(float deltaTime);
        void render();

        const std::vector<Component::GameObject*>& getGameObjects() const { return gameObjects; }
    };

} // namespace Scene

#endif // SCENE_HPP