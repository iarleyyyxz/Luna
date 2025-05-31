#pragma once
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include "Component.hpp"
#include "Transform2D.hpp" // Inclua o cabeçalho de Transform2D

class SceneObject : public std::enable_shared_from_this<SceneObject> {
public:
    SceneObject(const std::string& name) : name_(name) {
        // Todo SceneObject tem um Transform por padrão
        transform_ = Transform2D();
        transform_.SetActive(true);
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

    void Update(float deltaTime) {
        for (auto& comp : components_) comp->Update(deltaTime);
        for (auto& child : children_) child->Update(deltaTime);
    }

    void OnGUI() {
        if (ImGui::TreeNodeEx(name_.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (auto transformComp = GetComponent<Transform2D>())
            {
                transformComp->OnGui();
            }
            for (auto& comp : components_) comp->OnGui();
            for (auto& child : children_) child->OnGUI();
            ImGui::TreePop();
        }
    }

    // Novo método para acessar o Transform
    Transform2D& GetTransform() { return transform_; }
    const Transform2D& GetTransform() const { return transform_; }

private:
    std::string name_;
    std::weak_ptr<SceneObject> parent_;
    std::vector<std::shared_ptr<SceneObject>> children_;
    std::vector<std::shared_ptr<Component>> components_;
    Transform2D transform_; // Cada SceneObject agora tem um Transform
};