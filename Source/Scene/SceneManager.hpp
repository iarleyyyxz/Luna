#ifndef LUNA_SCENEMANAGER_HPP
#define LUNA_SCENEMANAGER_HPP

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

#include "Scene.hpp"
#include "Renderer2D.hpp"
#include "Source/Ecs/Entity.hpp"

namespace Luna {

    class SceneManager {
    public:
        SceneManager();
        ~SceneManager();

        bool Init();
        void Shutdown();

        void setRenderer(Renderer2D* renderer) { m_renderer = renderer; } // <-- Definição inline
        void setScreenSize(float width, float height) { m_screenWidth = width; m_screenHeight = height; } // <-- Definição inline

        void addScene(const std::string& name);
        void loadScene(const std::string& name);
        void removeScene(const std::string& name);
        Scene* getCurrentScene();

        Ecs::Entity createEntity();
        void destroyEntity(Ecs::Entity entity);

        template <typename T, typename... Args>
        T& addComponent(Ecs::Entity entity, Args&&... args);

        template <typename T>
        T* getComponent(Ecs::Entity entity) const;

        template <typename T>
        bool hasComponent(Ecs::Entity entity) const;

        void UpdateCurrentScene(float deltaTime);
        void RenderCurrentScene();
        void OnImGuiRender();

    private:
        std::unordered_map<std::string, std::unique_ptr<Scene>> m_scenes;
        std::unique_ptr<Scene> m_currentScene;
        Renderer2D* m_renderer;
        float m_screenWidth;
        float m_screenHeight;

    public:
        static char newSceneName[128];
        static std::string saveDirectory;
#ifdef _WIN32
        std::string openFolderDialog();
#else
        std::string openFolderDialog();
#endif
    };

} // namespace Luna

#endif // LUNA_SCENEMANAGER_HPP