#include "Float3.hpp"

#include <imgui.h>

namespace imguiex {
Float3::Float3(const std::string& label, float step)
    : label(label), value(), step(step), min(-1000), max(1000) {}

void Float3::draw() {
        ImGui::DragFloat3(label.c_str(), &value.x, step, min, max);
}
}  // namespace imguiex