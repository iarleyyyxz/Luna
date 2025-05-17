#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include <string>
#include <vector>
#include "component.hpp"

namespace Component {

    class GameObject {
    private:
        std::string name;
        std::vector<Component*> components;

    public:
        GameObject(const std::string& name);
        ~GameObject();

        void addComponent(Component* component);
        template <typename T>
        T* getComponent();
        void update(float deltaTime);
        void render();

        const std::string& getName() const { return name; }
    };

} // namespace Component

#endif // GAMEOBJECT_HPP