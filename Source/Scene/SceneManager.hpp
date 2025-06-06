#pragma once

#include <string>
#include <memory> // Para std::shared_ptr
#include <map>    // Para std::map de cenas

#include "Scene.hpp" // Inclui a definição de Scene
#include "Source/Renderer/Renderer2D.hpp" // Inclui Renderer2D para o método Render

class SceneManager {
public:
    // Retorna a única instância do SceneManager (padrão Singleton).
    static SceneManager& GetInstance();

    // Carrega uma cena pelo seu nome.
    // Se a cena já existir, ela será definida como a cena atual.
    // Se não existir, uma nova cena será criada e definida como atual.
    void LoadScene(const std::string& sceneName);

    // Retorna a cena atualmente ativa.
    // Retorna nullptr se nenhuma cena estiver ativa.
    std::shared_ptr<Scene> GetCurrentScene() const;

    // Atualiza a cena atualmente ativa.
    void Update(float deltaTime);

    // Renderiza a cena atualmente ativa.
    void Render(Renderer2D& renderer);

    // Desenha elementos da UI da cena atualmente ativa.
    void OnGui();

    // Destrutor padrão.
    ~SceneManager() = default;

private:
    // Construtor privado para implementar o padrão Singleton.
    // Impede a criação de instâncias externas.
    SceneManager() = default;

    // Apaga o construtor de cópia e o operador de atribuição para evitar cópias.
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;

    std::shared_ptr<Scene> m_currentScene; // A cena que está atualmente ativa
    std::map<std::string, std::shared_ptr<Scene>> m_scenes; // Map de todas as cenas carregadas
};
