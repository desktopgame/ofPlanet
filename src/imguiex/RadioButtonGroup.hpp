#pragma once
#ifndef IMGUIEX_RADIOBUTTONGROUP_HPP
#define IMGUIEX_RADIOBUTTONGROUP_HPP
#include <vector>
#include <string>

namespace imguiex {

class RadioButtonGroup {
public:
	explicit RadioButtonGroup();
	void draw();
	std::vector<std::string> labels;
	int mode;
	bool sameLine;
private:
};
}
#endif