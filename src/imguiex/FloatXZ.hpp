#pragma once
#ifndef IMGUIEX_FLOATXZ_HPP
#define IMGUIEX_FLOATXZ_HPP
#include <string>
#include <glm/glm.hpp>

namespace imguiex {
class FloatXZ {
public:
	explicit FloatXZ(const std::string& label, float step);
	void draw();
	std::string label;
	glm::vec3 value;
	float step;
	float min;
	float max;
};
}
#endif