#pragma once
#ifndef IMGUIEX_FLOAT3_HPP
#define IMGUIEX_FLOAT3_HPP
#include <glm/glm.hpp>
#include <string>

namespace imguiex {
/**
 * Float3 は、ImGuiのラッパーです。
 */
class Float3 {
       public:
        explicit Float3(const std::string& label, float step);

        /**
         * GUIを描画して値を更新します。
         */
        void draw();

        std::string label;
        glm::vec3 value;
        float step;
        float min;
        float max;
};
}  // namespace imguiex
#endif