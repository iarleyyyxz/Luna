#pragma once
#include <memory>
#include "Source/Renderer/Renderer2D.hpp"

class SceneObject;

class Component {
public:
    virtual ~Component() = default;
    virtual void Update(float deltaTime) {}
    virtual void Render(Renderer2D& renderer2D) {}
    virtual void OnGui() {} // Para integração com UI (como ImGui)

    void SetOwner(std::shared_ptr<SceneObject> owner) { owner_ = owner; }
    std::shared_ptr<SceneObject> GetOwner() const { return owner_.lock(); }

protected:
    std::weak_ptr<SceneObject> owner_;
};
