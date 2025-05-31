// SceneManager.cpp
#include "SceneManager.hpp"
#include <iostream>

void SceneManager::AddScene(std::shared_ptr<Scene> scene) {
    if (scenes.find(scene->GetName()) == scenes.end()) {
        scenes[scene->GetName()] = scene;
        std::cout << "Scene added: " << scene->GetName() << std::endl;
    }
    else {
        std::cerr << "Warning: Scene with name '" << scene->GetName() << "' already exists." << std::endl;
    }
}

void SceneManager::ChangeScene(const std::string& name) {
    auto it = scenes.find(name);
    if (it != scenes.end()) {
        std::cout << "Changing scene to: " << name << std::endl;
        if (currentScene) {
            std::cout << "Unloading previous scene: " << currentScene->GetName() << std::endl;
            currentScene->Unload();
        }
        currentScene = it->second;
        std::cout << "Loading new scene: " << currentScene->GetName() << std::endl;
        currentScene->Render(); // Assuming 'Render' now acts like 'OnLoad'
    }
    else {
        std::cerr << "Error: Scene with name '" << name << "' not found." << std::endl;
    }
}

void SceneManager::Update(float deltaTime) {
    if (currentScene) {
        currentScene->Update(deltaTime);
    }
}

void SceneManager::Render() {
    if (currentScene) {
        currentScene->Render();
    }
}

void SceneManager::RenderImGui() {
    if (currentScene) {
        currentScene->RenderImGui();
    }
}

std::shared_ptr<Scene> SceneManager::GetScene(const std::string& name) const {
    auto it = scenes.find(name);
    if (it != scenes.end()) {
        return it->second;
    }
    std::cerr << "Error: Scene with name '" << name << "' not found." << std::endl;
    return nullptr;
}
