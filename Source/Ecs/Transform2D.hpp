#pragma once
#include "Component.hpp" // Inclui Component
#include <glm.hpp>   // Para glm::vec2, glm::vec3
#include <imgui.h>       // Para ImGui

class Transform2D : public Component {
public:
    // Construtor: Inicializa com um nome padrão para o componente.
    // Como é um membro direto de SceneObject, o owner não será setado aqui, mas isso é aceitável.
    Transform2D() : Component(), position(0.0f), scale(1.0f), rotation(0.0f), active(true) {}

    glm::vec2 position;
    glm::vec3 scale = glm::vec3(1.0f);
    glm::vec3 rotation; // Euler angles (degrees). Considerando apenas o eixo Z para 2D.

    void OnGui() override {
        if (ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            // Usar DragFloat2 para glm::vec2 position
            ImGui::DragFloat2("Position", &position.x, 0.1f);
            // Usar DragFloat para rotation (assumindo rotação em Z para 2D)
            ImGui::DragFloat("Rotation (Z)", &rotation.z, 1.0f, 0.0f, 360.0f); // Editar apenas Z
            ImGui::DragFloat3("Scale", &scale.x, 0.1f);
            ImGui::TreePop();
        }
    }

    bool IsActive() const { return active; }
    void SetActive(bool act) { active = act; }

private:
    bool active;
};
