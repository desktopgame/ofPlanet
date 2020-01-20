#include "ListBox.hpp"

#include <imgui.h>

#include <cstring>

namespace imguiex {
ListBox::ListBox(const std::string& label)
    : items(), selectedIndex(0), label(label), raw() {
        rehash();
}
void ListBox::draw() {
        ImGui::ListBox(label.c_str(), &selectedIndex, raw.data(),
                       static_cast<int>(items.size()), 4);
}
void ListBox::rehash() {
        auto iter = raw.begin();
        while (iter != raw.end()) {
                std::free(*iter);
                ++iter;
        }
        raw.clear();
        for (auto str : items) {
                raw.emplace_back(_strdup(str.c_str()));
        }
        this->selectedIndex = 0;
}
}  // namespace imguiex