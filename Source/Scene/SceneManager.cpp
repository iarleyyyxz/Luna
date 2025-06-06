#include "SceneManager.hpp"
#include <iostream> // Para mensagens de log/erro
#include <imgui.h> // Para ImGui

// Inicializa a instância estática do SceneManager
SceneManager& SceneManager::GetInstance() {
    static SceneManager instance;
    return instance;
}

void SceneManager::LoadScene(const std::string& sceneName) {
    // Tenta encontrar a cena no map de cenas já carregadas
    auto it = m_scenes.find(sceneName);
    if (it != m_scenes.end()) {
        // Se a cena já existe, apenas a define como a cena atual
        m_currentScene = it->second;
        std::cout << "INFO::SCENE_MANAGER: Cena '" << sceneName << "' já carregada. Definida como cena atual." << std::endl;
    }
    else {
        // Se a cena não existe, cria uma nova e a adiciona ao map
        std::shared_ptr<Scene> newScene = std::make_shared<Scene>(sceneName);
        m_scenes[sceneName] = newScene;
        m_currentScene = newScene;
        std::cout << "INFO::SCENE_MANAGER: Nova cena '" << sceneName << "' criada e definida como cena atual." << std::endl;
    }
}

std::shared_ptr<Scene> SceneManager::GetCurrentScene() const {
    return m_currentScene;
}

void SceneManager::Update(float deltaTime) {
    if (m_currentScene) {
        m_currentScene->Update(deltaTime);
    }
}

void SceneManager::Render(Renderer2D& renderer) {
    if (m_currentScene) {
        m_currentScene->Render(renderer);
    }
}

void SceneManager::OnGui() {
    // Desenha o GUI para o SceneManager (pode listar cenas, etc.)
    if (ImGui::Begin("Scene Manager")) {


        if (m_currentScene) {
            ImGui::Text("Current Scene: %s", m_currentScene->GetName().c_str());
            ImGui::Separator();
            m_currentScene->OnGui(); // Chama o OnGui da cena atual
        }
        else {
            ImGui::Text("No active scene.");
        }

        ImGui::Separator();
        ImGui::Text("Available Scenes:");
        for (const auto& pair : m_scenes) {
            if (ImGui::Button(pair.first.c_str())) {
                LoadScene(pair.first); // Botão para carregar uma cena existente
            }
        }
        ImGui::End();
    }
}
