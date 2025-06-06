#pragma once
#include <string>
#include <vector>
#include <memory>
#include <algorithm> // Para std::remove_if
#include "Component.hpp"
#include "Transform2D.hpp" // Inclua o cabe�alho de Transform2D para a defini��o completa
#include <imgui.h> // Para ImGui::TreeNodeEx, ImGui::TreePop, etc. (se usado aqui)

// Forward declaration para Renderer2D, se SceneObject.hpp precisar dela diretamente
class Renderer2D;

class SceneObject : public std::enable_shared_from_this<SceneObject> {
public:
    SceneObject(const std::string& name) : name_(name) {
        // Transform2D j� � um membro direto, n�o precisa ser adicionado como um componente aqui.
        // A sua inst�ncia transform_ j� existe.
        // N�o chame AddComponent(std::make_shared<Transform2D>()); aqui.
        transform_.SetActive(true); // Garante que o transform est� ativo por padr�o
    }

    const std::string& GetName() const { return name_; }
    void SetName(const std::string& name) { name_ = name; }

    void SetParent(std::shared_ptr<SceneObject> parent) { parent_ = parent; }
    std::shared_ptr<SceneObject> GetParent() const { return parent_.lock(); }

    void AddChild(std::shared_ptr<SceneObject> child) {
        child->SetParent(shared_from_this());
        children_.push_back(child);
    }

    void RemoveChild(const std::string& name) {
        children_.erase(std::remove_if(children_.begin(), children_.end(),
            [&](std::shared_ptr<SceneObject>& child) { return child->GetName() == name; }),
            children_.end());
    }

    void AddComponent(std::shared_ptr<Component> component) {
        // Define o propriet�rio do componente para esta SceneObject.
        component->SetOwner(shared_from_this());
        components_.push_back(component);
    }

    template<typename T>
    std::shared_ptr<T> GetComponent() {
        for (auto& comp : components_) {
            if (auto cast = std::dynamic_pointer_cast<T>(comp)) {
                return cast;
            }
        }
        return nullptr;
    }

    // Sobrecarga const para GetComponent
    template<typename T>
    std::shared_ptr<const T> GetComponent() const {
        for (const auto& comp : components_) {
            if (auto cast = std::dynamic_pointer_cast<const T>(comp)) {
                return cast;
            }
        }
        return nullptr;
    }

    void Update(float deltaTime) {
        // Atualiza o Transform2D diretamente, pois � um membro.
        transform_.Update(deltaTime); // Se Transform2D tiver um m�todo Update

        // Atualiza os outros componentes
        for (auto& comp : components_) comp->Update(deltaTime);

        // Atualiza os filhos recursivamente
        for (auto& child : children_) child->Update(deltaTime);
    }

    // O m�todo Render de SceneObject deve passar o Renderer2D para os seus componentes de renderiza��o.
    void Render(Renderer2D& renderer) {
        // Para componentes que renderizam diretamente, chame seu Render.
        // O SpriteRenderer � o principal candidato aqui.
        for (auto& comp : components_) {
            comp->Render(renderer);
        }
        // Renderiza os filhos recursivamente
        for (auto& child : children_) child->Render(renderer);
    }

    void OnGui() {
        if (ImGui::TreeNodeEx(name_.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
        {
            // O Transform2D � um membro direto, ent�o chame OnGui diretamente nele.
            transform_.OnGui();

            // Desenha o GUI para os outros componentes
            for (auto& comp : components_) comp->OnGui();

            // Desenha o GUI para os filhos recursivamente
            for (auto& child : children_) child->OnGui();
            ImGui::TreePop();
        }
    }

    // Novo m�todo para acessar o Transform (agora que � um membro direto)
    Transform2D& GetTransform() { return transform_; }
    const Transform2D& GetTransform() const { return transform_; }

private:
    std::string name_;
    std::weak_ptr<SceneObject> parent_;
    std::vector<std::shared_ptr<SceneObject>> children_;
    std::vector<std::shared_ptr<Component>> components_;
    Transform2D transform_; // Cada SceneObject tem um Transform diretamente
};