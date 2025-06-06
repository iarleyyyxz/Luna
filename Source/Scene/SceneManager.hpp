#pragma once

#include <string>
#include <memory> // Para std::shared_ptr
#include <map>    // Para std::map de cenas

#include "Scene.hpp" // Inclui a defini��o de Scene
#include "Source/Renderer/Renderer2D.hpp" // Inclui Renderer2D para o m�todo Render

class SceneManager {
public:
    // Retorna a �nica inst�ncia do SceneManager (padr�o Singleton).
    static SceneManager& GetInstance();

    // Carrega uma cena pelo seu nome.
    // Se a cena j� existir, ela ser� definida como a cena atual.
    // Se n�o existir, uma nova cena ser� criada e definida como atual.
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

    // Destrutor padr�o.
    ~SceneManager() = default;

private:
    // Construtor privado para implementar o padr�o Singleton.
    // Impede a cria��o de inst�ncias externas.
    SceneManager() = default;

    // Apaga o construtor de c�pia e o operador de atribui��o para evitar c�pias.
    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;

    std::shared_ptr<Scene> m_currentScene; // A cena que est� atualmente ativa
    std::map<std::string, std::shared_ptr<Scene>> m_scenes; // Map de todas as cenas carregadas
};
