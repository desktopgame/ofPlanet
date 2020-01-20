#pragma once
#ifndef IMGUIEX_FLOATXZ_HPP
#define IMGUIEX_FLOATXZ_HPP
#include <string>
#include <glm/glm.hpp>

namespace imguiex {
/**
 * FloatXZ は、ImGuiのラッパーであり、XZが同期するようになっています。
 */
class FloatXZ {
public:
	explicit FloatXZ(const std::string& label, float step);

	/**
	 * GUIを描画して値を更新します。
	 */
	void draw();

	std::string label;
	glm::vec3 value;
	float step;
	float min;
	float max;
};
}
#endif