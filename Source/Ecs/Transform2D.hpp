#include "Component.hpp"

#include <glm.hpp>  // Essential Class to transform 2d component
#include <imgui.h>		// Base to a move/size/... SceneObject

class Transform2D : public Component {

public:
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
	
	void OnGui() override {
		ImGui::DragFloat3("Position", &position.x);
		ImGui::DragFloat3("Rotation", &rotation.x);
		ImGui::DragFloat3("Scale", &scale.x, 0.1f);
	}
};