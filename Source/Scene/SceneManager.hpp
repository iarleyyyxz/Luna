// SceneManager.hpp
#ifndef SCENEMANAGER_HPP
#define SCENEMANAGER_HPP

#include <memory>
#include <string>
#include <vector>
#include <map>
#include "Scene.hpp"

class SceneManager {
private:
    std::shared_ptr<Scene> currentScene;
    std::map<std::string, std::shared_ptr<Scene>> scenes;

public:
    // --- Scene Handling ---
    void AddScene(std::shared_ptr<Scene> scene);
    void ChangeScene(const std::string& name);
    void Update(float deltaTime);
    void Render();
    void RenderImGui();
    std::shared_ptr<Scene> GetCurrentScene() const { return currentScene; }
    std::shared_ptr<Scene> GetScene(const std::string& name) const;
    // --- Ending Scene Handling ---
};

#endif // SCENEMANAGER_HPP