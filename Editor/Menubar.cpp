// MenuBar.cpp
#include "MenuBar.hpp"
#include <iostream>
#include <GLFW/glfw3.h> // Para glfwSetWindowShouldClose

namespace Luna {

    void MenuBar::AddMenu(const Menu& menu) {
        m_menus.push_back(menu);
    }

    void MenuBar::Draw() {
        if (ImGui::BeginMenuBar()) {
            for (const auto& menu : m_menus) {
                if (ImGui::BeginMenu(menu.label.c_str())) {
                    for (const auto& item : menu.items) {
                        if (ImGui::MenuItem(item.label.c_str(), item.shortcut.empty() ? nullptr : item.shortcut.c_str(), false, item.enabled)) {
                            if (item.callback) {
                                item.callback();
                            }
                        }
                    }
                    ImGui::EndMenu();
                }
            }
            ImGui::EndMenuBar();
        }
    }

} // namespace Luna