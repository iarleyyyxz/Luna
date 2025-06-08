#pragma once
#include "Component.hpp" // Inclui Component
#include <glm.hpp>   // Para glm::vec2, glm::vec3
#include <imgui.h>       // Para ImGui

#define ICON_FA_COG "\xef\x80\x93"        // U+f013
#define ICON_FA_ARROWS_ALT "\xef\x81\xb2" // U+f0b2
#define ICON_FA_EXPAND "\xef\x81\xa5"     // U+f065
#define ICON_FA_COMPASS "\xef\x85\xa8"    // U+f14e
#define ICON_FA_CROSSHAIRS "\xef\x80\x9b"
#define ICON_FA_SYNC "\xef\x81\xa1"
#define ICON_FA_ARROW_DOWN "\xef\x81\xa7"
#define ICON_FA_ARROW_RIGHT "\xef\x81\xa4"
#define ICON_FA_ARROWS_ALT "\xef\x81\xb2"
#define ICON_FA_RULER "\xef\x85\x84"
#define ICON_FA_CAMERA "\xef\x80\xb7"
class Transform2D : public Component {
public:
    // Construtor: Inicializa com um nome padrão para o componente.
    // Como é um membro direto de SceneObject, o owner não será setado aqui, mas isso é aceitável.
    Transform2D() : Component(), position(0.0f), scale(1.0f), rotation(0.0f), active(true) {}

    glm::vec2 position;
    glm::vec3 scale = glm::vec3(1.0f);
    glm::vec3 rotation; // Euler angles (degrees). Considerando apenas o eixo Z para 2D.

    void OnGui() override {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap;

        std::string label = std::string(ICON_FA_EXPAND) + " Transform##transform_node";

        if (ImGui::TreeNodeEx(label.c_str(), flags))
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
