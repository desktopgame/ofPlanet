#pragma once
#ifndef IMGUIEX_INPUTFIELD_HPP
#define IMGUIEX_INPUTFIELD_HPP
#include <imgui.h>

#include <string>

namespace imguiex {
/**
 * InputField は、ImGuiのラッパーです。
 */
template <int N = 255>
class InputField {
       public:
        explicit InputField(const std::string& label);

        /**
         * GUIを描画して値を更新します。
         */
        void draw();

        void setString(const std::string& str);
        std::string getString() const;

        std::string label;
        char buffer[N];

       private:
};

template <int N>
inline InputField<N>::InputField(const std::string& label)
    : label(label), buffer() {
        std::memset(buffer, '\0', N);
}
template <int N>
inline void InputField<N>::draw() {
        ImGui::InputText(label.c_str(), buffer, N);
}
template <int N>
inline void InputField<N>::setString(const std::string& str) {
        if (str.size() >= N) {
                throw std::logic_error("string is too longer");
        }
        int len = static_cast<int>(str.size());
        std::memset(buffer, '\0', N);
        for (int i = 0; i < len; i++) {
                buffer[i] = str[i];
        }
        buffer[len] = '\0';
}
template <int N>
inline std::string InputField<N>::getString() const {
        return std::string(buffer);
}
}  // namespace imguiex
#endif