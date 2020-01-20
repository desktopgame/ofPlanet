#pragma once
#ifndef COMMON_MOUSESCROLL_HPP
#define COMMON_MOUSESCROLL_HPP
#include <ofVec3f.h>
namespace planet {
/**
 * MouseScroll
 * は、直前のマウス座標と現在のマウス座標からどれだけ移動したかを計算するクラスです。
 */
class MouseScroll {
       public:
        explicit MouseScroll();
        /**
         * 有効化します。
         */
        void enable();
        /**
         * 無効化します。
         */
        void disable();
        /**
         * 移動量を計算します。
         */
        void update();
        /**
         * 移動量を計算します。
         * @return
         */
        glm::vec2 getAmount() const;

       private:
        bool enabled;
        glm::vec2 oldMousePos;
        glm::vec2 diffMousePos;
};
}  // namespace planet
#endif