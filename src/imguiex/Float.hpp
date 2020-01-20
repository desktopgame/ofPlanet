#pragma once
#ifndef IMGUIEX_FLOAT_HPP
#define IMGUIEX_FLOAT_HPP
#include <string>
namespace imguiex {
/**
 * Float は、ImGuiのラッパーです。
 */
class Float {
public:
	explicit Float(const std::string& label, float step);

	/**
	 * GUIを描画して値を更新します。
	 */
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