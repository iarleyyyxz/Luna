// MenuBar.hpp
#ifndef LUNA_MENUBAR_HPP
#define LUNA_MENUBAR_HPP

#include <string>
#include <functional>
#include <vector>
#include "imgui.h"

namespace Luna {

    struct MenuItem {
        std::string label;
        std::string shortcut;
        std::function<void()> callback;
        bool enabled = true;
    };

    struct Menu {
        std::string label;
        std::vector<MenuItem> items;
    };

    class MenuBar {
    public:
        MenuBar() = default;

        void AddMenu(const Menu& menu);

        void Draw();

    private:
        std::vector<Menu> m_menus;
    };

} // namespace Luna

#endif // LUNA_MENUBAR_HPP