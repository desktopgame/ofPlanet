#include "Float3.hpp"
#include <imgui.h>

namespace imguiex {
Float3::Float3(const std::string & label, float step) : label(label), value(), step(step), min(-1000), max(1000) {
}

void Float3::draw() {
	float ptr[3] = {
		value.x,
		value.y,
		value.x
	};
	ImGui::DragFloat3(label.c_str(), ptr, step, min, max);
	value.x = ptr[0];
	value.y = ptr[1];
	value.z = ptr[2];
}
}