#pragma once
#ifndef COMMON_FIRSTPERSONCONTROLLER_HPP
#define COMMON_FIRSTPERSONCONTROLLER_HPP
#include <string>

#include "../shader/Transform.hpp"
#include "MouseScroll.hpp"
#include "WASDMove.hpp"
namespace planet {

class FirstPersonController {
       public:
		enum class Mode {
		 Mouse,
		 Key
		};
        explicit FirstPersonController();
        void enable();
        void disable();
        void update();

        glm::vec3 getRotation() const;
        glm::vec3 getVelocity() const;
        bool isMoveLeft() const;
        bool isMoveRight() const;
        bool isMoveFront() const;
        bool isMoveBack() const;

		void setMode(const Mode mode);
		Mode getMode() const;

		void setMoveSpeed(float moveSpeed);
		float getMoveSpeed() const;

		void setJumpSpeed(float jumpSpeed);
		float getJumpSpeed() const;

		void setRotateSpeed(float rotateSpeed);
		float getRotateSpeed() const;

		Transform& getTransform();
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