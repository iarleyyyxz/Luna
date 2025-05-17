#include "gameObject.hpp"

namespace Component {

    GameObject::GameObject(const std::string& name) : name(name) {}

    GameObject::~GameObject() {
        for (Component* component : components) {
            delete component;
        }
        components.clear();
    }

    void GameObject::addComponent(Component* component) {
        components.push_back(component);
    }

    template <typename T>
    T* GameObject::getComponent() {
        for (Component* component : components) {
            if (dynamic_cast<T*>(component)) {
                return static_cast<T*>(component);
            }
        }
        return nullptr;
    }

    void GameObject::update(float deltaTime) {
        for (Component* component : components) {
            component->update(deltaTime);
        }
    }

    void GameObject::render() {
        for (Component* component : components) {
            component->render();
        }
    }

    // Explicit template instantiation (para evitar erros de linker)
   // template class GameObject::getComponent<Component>();

} // namespace Component