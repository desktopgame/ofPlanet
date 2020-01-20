#pragma once
#ifndef IMGUIEX_FLOAT2_HPP
#define IMGUIEX_FLOAT2_HPP
#include <glm/glm.hpp>
#include <string>

namespace imguiex {
/**
 * Float2 は、ImGuiのラッパーです。
 */
class Float2 {
       public:
        explicit Float2(const std::string& label, float step);

        /**
         * GUIを描画して値を更新します。
         */
        void draw();

        std::string label;
        glm::vec2 value;
        float step;
        float min;
        float max;

       private:
};
}  // namespace imguiex
#endif