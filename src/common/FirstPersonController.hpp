#pragma once
#ifndef COMMON_FIRSTPERSONCONTROLLER_HPP
#define COMMON_FIRSTPERSONCONTROLLER_HPP
#include <string>

#include "Transform.hpp"
#include "MouseScroll.hpp"
#include "WASDMove.hpp"
namespace planet {
/**
 * FirstPersonController は、一人称のカメラ制御を行うためのクラスです。
 */
class FirstPersonController {
       public:
        enum class Mode { Mouse, Key };
        explicit FirstPersonController();

        /**
         * 有効化します。
         */
        void enable();

        /**
         * 無効化します。
         */
        void disable();

        /**
         * 入力をチェックして現在の進行方向や力を計算します。
         */
        void update();

        /**
         * 現在の回転を返します。
         * @return
         */
        glm::vec3 getRotation() const;

        /**
         * 現在の方向を返します。
         * @return
         */
        glm::vec3 getVelocity() const;

        /**
         * 左に移動しているなら true を返します。
         * @return
         */
        bool isMoveLeft() const;

        /**
         * 右に移動しているなら true を返します。
         * @return
         */
        bool isMoveRight() const;

        /**
         * 前に移動しているなら true を返します。
         * @return
         */
        bool isMoveFront() const;

        /**
         * 後ろに移動しているなら true を返します。
         * @return
         */
        bool isMoveBack() const;

        /**
         * モードを設定します。
         * @param mode
         */
        void setMode(const Mode mode);

        /**
         * 現在のモードを返します。
         * @return
         */
        Mode getMode() const;

        /**
         * 移動速度を設定します。
         * @param moveSpeed
         */
        void setMoveSpeed(float moveSpeed);

        /**
         * 移動速度を返します。
         * @return
         */
        float getMoveSpeed() const;

        /**
         * ジャンプ力を設定します。
         * @param jumpSpeed
         */
        void setJumpSpeed(float jumpSpeed);

        /**
         * ジャンプ力を返します。
         * @return
         */
        float getJumpSpeed() const;

        /**
         * 回転速度を設定します。
         * @param rotateSpeed
         */
        void setRotateSpeed(float rotateSpeed);

        /**
         * 回転速度を返します。
         * @return
         */
        float getRotateSpeed() const;

        /**
         * @return
         */
        Transform& getTransform();

        /**
         * @return
         */
        const Transform& getTransform() const;

       private:
        Mode mode;
        glm::vec3 velocity;
        Transform transform;
        MouseScroll mouseScroll;
        WASDMove wasdMove;
        float moveSpeed;
        float jumpSpeed;
        float rotateSpeed;
        bool moveLeft, moveRight, moveFront, moveBack;
};
}  // namespace planet
#endif