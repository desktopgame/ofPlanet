#pragma once
#ifndef IMGUIEX_RADIOBUTTONGROUP_HPP
#define IMGUIEX_RADIOBUTTONGROUP_HPP
#include <string>
#include <vector>

namespace imguiex {
/**
 * RadioButtonGroup は、ImGuiのラッパーです。
 */
class RadioButtonGroup {
       public:
        explicit RadioButtonGroup();

        /**
         * GUIを描画して値を更新します。
         */
        void draw();

        std::vector<std::string> labels;
        int mode;
        bool sameLine;

       private:
};
}  // namespace imguiex
#endif