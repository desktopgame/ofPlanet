#pragma once
#ifndef IMGUIEX_LISTBOX_HPP
#define IMGUIEX_LISTBOX_HPP
#include <vector>
#include <string>

namespace imguiex {
/**
 * ListBox は、ImGuiのラッパーです。
 */
class ListBox {
public:
	explicit ListBox(const std::string& label);

	/**
	 * GUIを描画して値を更新します。
	 */
	void draw();

	/**
	 * 現在の items の値から文字列を生成します。
	 * items を変更したあとは必ず呼び出してください。
	 */
	void rehash();

	std::vector<std::string> items;
	int selectedIndex;
	std::string label;
private:
	std::vector<char*> raw;
};
}
#endif