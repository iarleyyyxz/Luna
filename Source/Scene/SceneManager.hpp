#ifndef SCENE_MANAGER_HPP
#define SCENE_MANAGER_HPP

#include <string>
#include <unordered_map>
#include <memory> // Para std::unique_ptr

#include "Scene.hpp" // Inclui a classe base Scene

// Forward declarations para evitar inclus�es circulares
class Renderer2D;
class Keyboard;
class Mouse;

class SceneManager {
public:
    SceneManager();
    ~SceneManager();

    // Inicializa o SceneManager (n�o faz muito, apenas configura o estado inicial)
    bool Init();
    // Encerra o SceneManager, liberando a cena atual
    void Shutdown();

    // Adiciona uma cena ao gerenciador.
    // sceneName: Nome �nico para identificar a cena.
    // scene: Um unique_ptr para a inst�ncia da cena. O SceneManager assume a propriedade.
    void AddScene(const std::string& sceneName, std::unique_ptr<Scene> scene);

    // Alterna para uma cena espec�fica.
    // sceneName: O nome da cena para a qual alternar.
    // window, screenWidth, screenHeight: Par�metros necess�rios para inicializar a nova cena.
    // renderer, keyboard, mouse: Refer�ncias aos recursos da engine passados para a cena.
    bool SwitchToScene(const std::string& sceneName, GLFWwindow* window, float screenWidth, float screenHeight,
        Renderer2D& renderer, Keyboard& keyboard, Mouse& mouse);

    // Obt�m a cena atualmente ativa.
    Scene* GetCurrentScene() const { return m_currentScene.get(); }

private:
    // Mapa para armazenar todas as cenas registradas.
    // Usamos unique_ptr para garantir que o SceneManager � o �nico propriet�rio das cenas.
    std::unordered_map<std::string, std::unique_ptr<Scene>> m_scenes;

    // Ponteiro para a cena atualmente ativa.
    // N�o � um unique_ptr porque a propriedade j� est� em m_scenes.
    std::unique_ptr<Scene> m_currentScene;

    // Refer�ncias aos recursos da engine que ser�o passados para as cenas.
    // N�o s�o de propriedade do SceneManager.
    Renderer2D* m_rendererRef;
    Keyboard* m_keyboardRef;
    Mouse* m_mouseRef;
    GLFWwindow* m_windowRef;
    float m_screenWidthRef;
    float m_screenHeightRef;
};

#endif // SCENE_MANAGER_HPP
