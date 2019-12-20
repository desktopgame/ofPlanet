#pragma once
#ifndef IMGUIEX_CHECKBOX_HPP
#define IMGUIEX_CHECKBOX_HPP
#include <string>

namespace imguiex {
class CheckBox {
public:
	explicit CheckBox(const std::string& label);

	void draw();

	std::string label;
	bool enabled;
private:
};
}
#endif