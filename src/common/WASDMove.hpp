#pragma once
#ifndef COMMON_WASDMOVE_HPP
#define COMMON_WASDMOVE_HPP
#include <ofVec3f.h>
namespace planet {
/**
 * WASDMove
 * は、一人称視点のゲームにおけるキーボード操作で一般的なWASDをサポートします。
 */
class WASDMove {
       public:
        explicit WASDMove();

        /**
         * 有効化します。
         */
        void enable();

        /**
         * 無効化します。
         */
        void disable();

        /**
         * キー入力を検出します。
         */
        void update();

        /**
         * スプリント中なら 2 を、それ以外なら 1 を返します。
         * @return
         */
        float getScale() const;

        /**
         * 現在の移動方向を返します。
         * @return
         */
        glm::ivec2 getAxis() const;

        /**
         * スプリント中なら true を返します。
         */
        bool isSprint() const;

       private:
        bool enabled;
        bool sprint;
        glm::ivec2 axis;
};
}  // namespace planet
#endif