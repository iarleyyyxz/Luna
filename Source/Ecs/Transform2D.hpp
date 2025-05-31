#pragma once
#include "Component.hpp"
#include <glm.hpp>   // Essential Class to transform 2d component
#include <imgui.h>        // Base to a move/size/... SceneObject

class Transform2D : public Component {
public:
    glm::vec3 translation;
    glm::vec3 scale = glm::vec3(1.0f);
    glm::vec3 rotation; // Euler angles (degrees)

    Transform2D() : translation(0.0f), rotation(0.0f), active(true) {}

    void OnGui() override {
        if (ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::DragFloat3("Position", &translation.x, 0.1f);
            ImGui::DragFloat3("Rotation", &rotation.x, 1.0f); // Assuming degrees
            ImGui::DragFloat3("Scale", &scale.x, 0.1f);
            ImGui::TreePop();
        }
    }

    bool IsActive() const { return active; }
    void SetActive(bool act) { active = act; }

private:
    bool active;
};