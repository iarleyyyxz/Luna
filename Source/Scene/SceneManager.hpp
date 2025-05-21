#ifndef SCENE_MANAGER_HPP
#define SCENE_MANAGER_HPP

#include <string>
#include <unordered_map>
#include <memory> // Para std::unique_ptr

#include "Scene.hpp" // Inclui a classe base Scene

// Forward declarations para evitar inclusões circulares
class Renderer2D;
class Keyboard;
class Mouse;

class SceneManager {
public:
    SceneManager();
    ~SceneManager();

    // Inicializa o SceneManager (não faz muito, apenas configura o estado inicial)
    bool Init();
    // Encerra o SceneManager, liberando a cena atual
    void Shutdown();

    // Adiciona uma cena ao gerenciador.
    // sceneName: Nome único para identificar a cena.
    // scene: Um unique_ptr para a instância da cena. O SceneManager assume a propriedade.
    void AddScene(const std::string& sceneName, std::unique_ptr<Scene> scene);

    // Alterna para uma cena específica.
    // sceneName: O nome da cena para a qual alternar.
    // window, screenWidth, screenHeight: Parâmetros necessários para inicializar a nova cena.
    // renderer, keyboard, mouse: Referências aos recursos da engine passados para a cena.
    bool SwitchToScene(const std::string& sceneName, GLFWwindow* window, float screenWidth, float screenHeight,
        Renderer2D& renderer, Keyboard& keyboard, Mouse& mouse);

    // Obtém a cena atualmente ativa.
    Scene* GetCurrentScene() const { return m_currentScene.get(); }

private:
    // Mapa para armazenar todas as cenas registradas.
    // Usamos unique_ptr para garantir que o SceneManager é o único proprietário das cenas.
    std::unordered_map<std::string, std::unique_ptr<Scene>> m_scenes;

    // Ponteiro para a cena atualmente ativa.
    // Não é um unique_ptr porque a propriedade já está em m_scenes.
    std::unique_ptr<Scene> m_currentScene;

    // Referências aos recursos da engine que serão passados para as cenas.
    // Não são de propriedade do SceneManager.
    Renderer2D* m_rendererRef;
    Keyboard* m_keyboardRef;
    Mouse* m_mouseRef;
    GLFWwindow* m_windowRef;
    float m_screenWidthRef;
    float m_screenHeightRef;
};

#endif // SCENE_MANAGER_HPP
