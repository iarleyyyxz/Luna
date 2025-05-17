#ifndef GAME_HPP
#define GAME_HPP

#include <GLFW/glfw3.h>
#include "keyboard.hpp"
#include "mouse.hpp"
#include "Scene/scene.hpp" // Incluir a classe Scene

namespace Core {

    class Game {
    private:
        GLFWwindow* window;
        KeyListener* keyListener;
        MouseListener* mouseListener;
        Scene::Scene* currentScene; // Adicionar a cena atual
        float deltaTime;
        float lastFrame;

    public:
        Game(GLFWwindow* window, KeyListener* keyListener, MouseListener* mouseListener);
        ~Game();

        void run();
        void init();
        void update();
        void render();
        void shutdown();
        void loadScene(const std::string& filepath);

        float getDeltaTime() const { return deltaTime; }
        KeyListener* getKeyListener() const { return keyListener; }
        MouseListener* getMouseListener() const { return mouseListener; }
        Scene::Scene* getCurrentScene() const { return currentScene; }
        void setCurrentScene(Scene::Scene* scene);

    private:
        void loadResources();
        void initGameObjects();
    };

} // namespace Core

#endif // GAME_HPP