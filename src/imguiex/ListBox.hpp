#pragma once
#ifndef IMGUIEX_LISTBOX_HPP
#define IMGUIEX_LISTBOX_HPP
#include <vector>
#include <string>

namespace imguiex {
class ListBox {
public:
	explicit ListBox(const std::string& label);

	void draw();
	void rehash();

	std::vector<std::string> items;
	int selectedIndex;
	std::string label;
private:
	std::vector<char*> raw;
};
}
#endif