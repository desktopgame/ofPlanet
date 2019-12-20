#pragma once
#ifndef IMGUIEX_FLOAT_HPP
#define IMGUIEX_FLOAT_HPP
#include <string>

namespace imguiex {
class Float {
public:
	explicit Float(const std::string& label, float step);
	void draw();
	std::string label;
	float value;
	float step;
	float min;
	float max;
private:
};
}
#endif