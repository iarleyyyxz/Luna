// ViewportGui.hpp
#ifndef VIEWPORT_GUI_HPP
#define VIEWPORT_GUI_HPP

#include <glm.hpp>

#include "imgui.h"

class ViewportGui {
public:
    ViewportGui();
    void Render(unsigned int framebufferTexture);

    bool isFocused() const;
    bool isHovered() const;


private:
    // Adicione aqui quaisquer dados ou configurações específicas da Viewport
    bool isViewportFocused = false;
    bool isViewportHovered = false;
};

#endif // VIEWPORT_GUI_HPP