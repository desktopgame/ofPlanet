#include "Float2.hpp"
#include <imgui.h>

namespace imguiex {
Float2::Float2(const std::string & label, float step) : label(label), value(), step(step), min(-100), max(100) {
}

void Float2::draw() {
	ImGui::DragFloat2(label.c_str(), &value.x, step, min, max);
}
}