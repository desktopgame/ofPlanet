#include "FloatXZ.hpp"
#include <imgui.h>
#include <cmath>
#include <limits>

namespace imguiex {
FloatXZ::FloatXZ(const std::string & label, float step) : label(label), value(), step(step), min(-1000), max(1000) {
}

void FloatXZ::draw() {
	constexpr float epsilon = std::numeric_limits<float>::epsilon();
	glm::vec3 cp = value;
	ImGui::DragFloat3(label.c_str(), &cp.x, step, min, max);
	float diffX = std::abs(value.x-cp.x);
	float diffZ = std::abs(value.z-cp.z);
	if (std::abs(diffX - diffZ) <= epsilon) {
		return;
	}
	if (diffX > diffZ) {
		cp.z = cp.x;
	} else {
		cp.x = cp.z;
	}
	this->value = cp;
}
}