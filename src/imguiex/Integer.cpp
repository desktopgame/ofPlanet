#include "Integer.hpp"

#include <imgui.h>

namespace imguiex {
	Integer::Integer(const std::string& label, int step)
		: label(label), value(0), step(step), min(-100), max(100) {}

	void Integer::draw() { ImGui::DragInt(label.c_str(), &value, step, min, max); }
}  // namespace imguiex