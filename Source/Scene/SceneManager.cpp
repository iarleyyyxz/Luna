#include "SceneManager.hpp"
#include <iostream>

// Incluir os cabeçalhos completos aqui, pois estamos a usar as classes
#include "Renderer2D.hpp"


SceneManager::SceneManager()
    : m_currentScene(nullptr), m_rendererRef(nullptr), m_keyboardRef(nullptr), m_mouseRef(nullptr),
    m_windowRef(nullptr), m_screenWidthRef(0.0f), m_screenHeightRef(0.0f)
{
    std::cout << "SceneManager construído!" << std::endl;
}

SceneManager::~SceneManager() {
    Shutdown();
    std::cout << "SceneManager destruído!" << std::endl;
}

bool SceneManager::Init() {
    // Nenhuma inicialização complexa aqui, apenas para fins de estrutura.
    std::cout << "SceneManager inicializado." << std::endl;
    return true;
}

void SceneManager::Shutdown() {
    // Se houver uma cena ativa, desligue-a antes de limpar.
    if (m_currentScene) {
        m_currentScene->Shutdown();
    }
    // Limpar todas as cenas armazenadas no mapa.
    // unique_ptr garante que as cenas sejam deletadas automaticamente.
    m_scenes.clear();
    std::cout << "SceneManager encerrado." << std::endl;
}

void SceneManager::AddScene(const std::string& sceneName, std::unique_ptr<Scene> scene) {
    if (m_scenes.count(sceneName) > 0) {
        std::cerr << "AVISO::SCENE_MANAGER: Cena com o nome '" << sceneName << "' já existe. Substituindo." << std::endl;
    }
    m_scenes[sceneName] = std::move(scene); // Move a propriedade do unique_ptr
    std::cout << "Cena '" << sceneName << "' adicionada ao SceneManager." << std::endl;
}

bool SceneManager::SwitchToScene(const std::string& sceneName, GLFWwindow* window, float screenWidth, float screenHeight,
    Renderer2D& renderer, Keyboard& keyboard, Mouse& mouse) {
    auto it = m_scenes.find(sceneName);
    if (it == m_scenes.end()) {
        std::cerr << "ERRO::SCENE_MANAGER: Cena '" << sceneName << "' não encontrada." << std::endl;
        return false;
    }

    // Se houver uma cena ativa, desligue-a primeiro.
    if (m_currentScene) {
        m_currentScene->Shutdown();
        m_currentScene.reset(); // Libera o unique_ptr da cena anterior
    }

    // Define as referências para os recursos da engine
    m_windowRef = window;
    m_screenWidthRef = screenWidth;
    m_screenHeightRef = screenHeight;
    m_rendererRef = &renderer;
    m_keyboardRef = &keyboard;
    m_mouseRef = &mouse;

    // Move a cena do mapa para a cena atual.
    // Isso transfere a propriedade da cena para m_currentScene.
    m_currentScene = std::move(it->second);
    m_scenes.erase(it); // Remove a entrada do mapa, pois a cena foi movida

    // Inicializa a nova cena
    if (!m_currentScene->Init(m_windowRef, m_screenWidthRef, m_screenHeightRef,
        *m_rendererRef, *m_keyboardRef, *m_mouseRef)) {
        std::cerr << "ERRO::SCENE_MANAGER: Falha ao inicializar a cena '" << sceneName << "'." << std::endl;
        m_currentScene.reset(); // Limpa a cena se a inicialização falhar
        return false;
    }

    std::cout << "Alternado para a cena: '" << sceneName << "'." << std::endl;
    return true;
}