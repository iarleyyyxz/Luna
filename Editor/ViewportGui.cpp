// ViewportGui.cpp
#include "ViewportGui.hpp"
#include "imgui.h"

ViewportGui::ViewportGui() {
    // Inicialização da ViewportGui, se necessário
}

void ViewportGui::Render(unsigned int framebufferTexture) {
    ImGui::Begin("Viewport");

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    ImGui::Image((framebufferTexture), viewportPanelSize, ImVec2(0, 1), ImVec2(1, 0));

    isViewportFocused = ImGui::IsWindowFocused();
    isViewportHovered = ImGui::IsWindowHovered();

    ImGui::End();
}

bool ViewportGui::isFocused() const {
    return isViewportFocused;
}

bool ViewportGui::isHovered() const {
    return isViewportHovered;
}