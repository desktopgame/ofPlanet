#include "CheckBox.hpp"

#include <imgui.h>

namespace imguiex {
CheckBox::CheckBox(const std::string& label) : label(label), enabled(false) {}

void CheckBox::draw() {
        this->enabled = ImGui::Checkbox(label.c_str(), &enabled);
}
}  // namespace imguiex