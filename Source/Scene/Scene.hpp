#ifndef SCENE_HPP
#define SCENE_HPP

#include <string>
#include <iostream>
#include <vector>
#include "Source/Ecs/SceneObject.hpp"


class Scene {
public:
    Scene(const std::string& name) : m_name(name) {}

    virtual ~Scene() = default;

    const std::string& GetName() const { return m_name; }
    virtual void Render() { std::cout << "Scene '" << m_name << "' loaded.\n"; }
    virtual void Unload() { std::cout << "Scene '" << m_name << "' unloaded.\n"; }
    virtual void Update(float deltaTime);
    virtual void RenderImGui() {}

    const std::vector<SceneObject>& GetSceneObjects() const { return objects; }
    void AddSceneObject(const SceneObject& object);

protected:
    std::string m_name;
    std::vector<SceneObject> objects;
};

#endif // SCENE_HPP