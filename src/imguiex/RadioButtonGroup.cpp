#include "RadioButtonGroup.hpp"

#include <imgui.h>

namespace imguiex {
RadioButtonGroup::RadioButtonGroup() : labels(), mode(), sameLine(true) {}

void RadioButtonGroup::draw() {
        for (int i = 0; i < static_cast<int>(labels.size()); i++) {
                ImGui::RadioButton(labels.at(i).c_str(), &mode, i);
                if (sameLine && i != labels.size() - 1) {
                        ImGui::SameLine();
                }
        }
}
}  // namespace imguiex