#pragma once
#ifndef IMGUIEX_INTEGER_HPP
#define IMGUIEX_INTEGER_HPP
#include <string>
namespace imguiex {
/**
 * Integer は、ImGuiのラッパーです。
 */
class Integer {
       public:
        explicit Integer(const std::string& label, int step);

        /**
         * GUIを描画して値を更新します。
         */
        void draw();

        std::string label;
        int value;
        int step;
        int min;
        int max;

       private:
};
}  // namespace imguiex
#endif