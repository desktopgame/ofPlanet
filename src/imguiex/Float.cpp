#include "Float.hpp"

#include <imgui.h>

namespace imguiex {
Float::Float(const std::string& label, float step)
    : label(label), value(0), step(step), min(-100), max(100) {}

void Float::draw() { ImGui::DragFloat(label.c_str(), &value, step, min, max); }
}  // namespace imguiex