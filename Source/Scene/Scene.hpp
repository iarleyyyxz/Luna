#pragma once

#include <string>
#include <vector>
#include <memory> // Para std::shared_ptr
#include <algorithm> // Para std::remove_if

#include "Source/ECS/SceneObject.hpp" // Inclui a defini��o de SceneObject
#include "Source/Renderer/Renderer2D.hpp" // Inclui Renderer2D para o m�todo Render

// Forward declaration para evitar depend�ncia circular se SceneObject precisar de Scene
// (N�o � o caso aqui, mas � boa pr�tica para SceneManager)
class Renderer2D;

class Scene {
public:
    // Construtor: Uma cena � criada com um nome.
    Scene(const std::string& name);

    // Destrutor padr�o.
    ~Scene() = default;

    // Retorna o nome da cena.
    const std::string& GetName() const { return m_name; }

    // Adiciona um SceneObject � cena.
    // O SceneObject deve ser um shared_ptr.
    void AddSceneObject(std::shared_ptr<SceneObject> object);

    // Remove um SceneObject da cena pelo seu nome.
    void RemoveSceneObject(const std::string& name);

    // Obt�m um SceneObject da cena pelo seu nome.
    // Retorna nullptr se n�o encontrado.
    std::shared_ptr<SceneObject> GetSceneObjectByName(const std::string& name);

    // Atualiza todos os SceneObjects na cena.
    // deltaTime: tempo decorrido desde a �ltima atualiza��o.
    void Update(float deltaTime);

    // Renderiza todos os SceneObjects na cena.
    // renderer: A inst�ncia do Renderer2D a ser usada para o desenho.
    void Render(Renderer2D& renderer);

    // M�todo para desenhar elementos da UI da cena (usando ImGui, por exemplo).
    void OnGui();

private:
    std::string m_name; // Nome da cena
    // Vetor de shared_ptr para os SceneObjects de n�vel raiz na cena.
    // SceneObjects com pais ser�o gerenciados por seus pais.
    std::vector<std::shared_ptr<SceneObject>> m_rootObjects;
};
