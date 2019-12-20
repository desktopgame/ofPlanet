#pragma once
#ifndef IMGUIEX_INPUTFIELD_HPP
#define IMGUIEX_INPUTFIELD_HPP
#include <imgui.h>
#include <string>

namespace imguiex {
template<int N=255>
class InputField {
public:
	explicit InputField(const std::string& label);

	void draw();
	std::string getString() const;

	std::string label;
	char buffer[N];
private:
};

template<int N>
inline InputField<N>::InputField(const std::string & label) : label(label), buffer() {
	std::memset(buffer, '\0', N);
}
template<int N>
inline void InputField<N>::draw() {
	ImGui::InputText(label.c_str(), buffer, N);
}
template<int N>
inline std::string InputField<N>::getString() const {
	return std::string(buffer);
}
}
#endif