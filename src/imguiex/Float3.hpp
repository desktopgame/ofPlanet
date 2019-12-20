#pragma once
#ifndef IMGUIEX_FLOAT3_HPP
#define IMGUIEX_FLOAT3_HPP
#include <string>
#include <glm/glm.hpp>

namespace imguiex {
class Float3 {
public:
	explicit Float3(const std::string& label, float step);
	void draw();
	std::string label;
	glm::vec3 value;
	float step;
	float min;
	float max;
};
}
#endif