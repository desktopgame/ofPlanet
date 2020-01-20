#pragma once
#ifndef IMGUIEX_CHECKBOX_HPP
#define IMGUIEX_CHECKBOX_HPP
#include <string>

namespace imguiex {
/**
 * CheckBox は、ImGuiのラッパーです。
 */
class CheckBox {
       public:
        explicit CheckBox(const std::string& label);

        /**
         * GUIを描画して、値を更新します。
         */
        void draw();

        std::string label;
        bool enabled;

       private:
};
}  // namespace imguiex
#endif