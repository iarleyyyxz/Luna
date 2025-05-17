#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.hpp" // Seu header
#include "Scene/scene.hpp"
#include "Component/gameObject.hpp"
#include <iostream>

namespace Core {

    Game::Game(GLFWwindow* window, KeyListener* keyListener, MouseListener* mouseListener)
        : window(window), keyListener(keyListener), mouseListener(mouseListener), currentScene(nullptr) {
        init();
    }

    Game::~Game() {
        shutdown();
        if (currentScene) {
            delete currentScene;
        }
    }

    void Game::run() {
        lastFrame = 0.0f;
        while (!glfwWindowShouldClose(window)) {
            float currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            glfwPollEvents();
            update();
            render();

            glfwSwapBuffers(window);
        }
    }

    void Game::init() {
        loadResources();
        initGameObjects();
       
    }

    void Game::update() {
        
        if (currentScene) {
            currentScene->update(deltaTime);
        }
        // Atualizar lógica do editor aqui
    }

    void Game::render() {
      
        if (currentScene) {
            currentScene->render();
        }
    }

    void Game::shutdown() {
        glfwTerminate();
       
    }

    void Game::loadResources() {
        std::cout << "Carregando recursos do editor..." << std::endl;
    }

    void Game::initGameObjects() {
        std::cout << "Inicializando objetos do editor..." << std::endl;
        // Inicializar objetos da UI do editor, etc.
    }

    void Game::loadScene(const std::string& filepath) {
        // No futuro, leremos de um arquivo. Por agora, criamos uma cena vazia.
        currentScene = new Scene::Scene();
        std::cout << "Cena carregada (simulada): " << filepath << std::endl;
    }

    void Game::setCurrentScene(Scene::Scene* scene) {
        if (currentScene) {
            delete currentScene;
        }
        currentScene = scene;
    }

} // namespace Core