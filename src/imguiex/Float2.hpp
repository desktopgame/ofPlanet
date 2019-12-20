#pragma once
#ifndef IMGUIEX_FLOAT2_HPP
#define IMGUIEX_FLOAT2_HPP
#include <string>
#include <glm/glm.hpp>

namespace imguiex {
class Float2 {
public:
	explicit Float2(const std::string& label, float step);
	void draw();
	std::string label;
	glm::vec2 value;
	float step;
	float min;
	float max;
private:
};
}
#endif