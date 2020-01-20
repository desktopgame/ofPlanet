#include "FirstPersonController.hpp"

#include <ofBaseApp.h>
#include <ofEvents.h>
#include <ofMath.h>

#include "../shader/Transform.hpp"
#include "glfw.hpp"
namespace planet {

FirstPersonController::FirstPersonController()
    : mode(Mode::Mouse),
      mouseScroll(),
      wasdMove(),
      transform(),
      moveSpeed(0.2f),
      jumpSpeed(0.6f),
      rotateSpeed(0.1f),
      velocity() {}

void FirstPersonController::enable() {
        mouseScroll.enable();
        wasdMove.enable();
}

void FirstPersonController::disable() {
        mouseScroll.disable();
        wasdMove.disable();
}

void FirstPersonController::update() {
        mouseScroll.update();
        wasdMove.update();
        glm::ivec2 axis = wasdMove.getAxis();
        this->moveLeft = this->moveRight = this->moveFront = this->moveBack =
            false;
        // ‰ñ“]î•ñ‚ÌÝ’è
        if (this->mode == Mode::Mouse) {
                transform.rotation.x += mouseScroll.getAmount().x * rotateSpeed;
                transform.rotation.y += mouseScroll.getAmount().y * rotateSpeed;
        } else {
                if (ofGetKeyPressed(OF_KEY_LEFT)) {
                        transform.rotation.x += rotateSpeed;
                } else if (ofGetKeyPressed(OF_KEY_RIGHT)) {
                        transform.rotation.x -= rotateSpeed;
                }
                if (ofGetKeyPressed(OF_KEY_UP)) {
                        transform.rotation.y += rotateSpeed;
                } else if (ofGetKeyPressed(OF_KEY_DOWN)) {
                        transform.rotation.y -= rotateSpeed;
                }
        }
        transform.rotation.y = ofClamp(transform.rotation.y, -90.0f, 90.0f);

        if (transform.rotation.x > 360) {
                transform.rotation.x -= 360;
        }
        if (transform.rotation.y > 360) {
                transform.rotation.y -= 360;
        }
        if (transform.rotation.z > 360) {
                transform.rotation.z -= 360;
        }
        // ˆÚ“®•ûŒü‚ÌÝ’è
        this->velocity = glm::vec3();
        glm::vec3 force = glm::vec3(wasdMove.getScale() * moveSpeed, 0,
                                    wasdMove.getScale() * moveSpeed);
        if (axis.y == 1) {
                this->velocity = transform.forward() * force;
                this->moveFront = true;
        } else if (axis.y == -1) {
                this->velocity = transform.backward() * force;
                this->moveBack = true;
        }
        if (axis.x == -1) {
                this->velocity = transform.left() * force;
                this->moveLeft = true;
        } else if (axis.x == 1) {
                this->velocity = transform.right() * force;
                this->moveBack = true;
        }
}

glm::vec3 FirstPersonController::getRotation() const {
        return transform.rotation;
}

glm::vec3 FirstPersonController::getVelocity() const { return velocity; }

bool FirstPersonController::isMoveLeft() const { return moveLeft; }

bool FirstPersonController::isMoveRight() const { return moveRight; }

bool FirstPersonController::isMoveFront() const { return moveFront; }

bool FirstPersonController::isMoveBack() const { return moveBack; }
void FirstPersonController::setMode(const Mode mode) { this->mode = mode; }
FirstPersonController::Mode FirstPersonController::getMode() const {
        return this->mode;
}
void FirstPersonController::setMoveSpeed(float moveSpeed) {
        this->moveSpeed = moveSpeed;
}
float FirstPersonController::getMoveSpeed() const { return moveSpeed; }
void FirstPersonController::setJumpSpeed(float jumpSpeed) {
        this->jumpSpeed = jumpSpeed;
}
float FirstPersonController::getJumpSpeed() const { return this->jumpSpeed; }
void FirstPersonController::setRotateSpeed(float rotateSpeed) {
        this->rotateSpeed = rotateSpeed;
}
float FirstPersonController::getRotateSpeed() const {
        return this->rotateSpeed;
}
Transform& FirstPersonController::getTransform() { return this->transform; }
const Transform& FirstPersonController::getTransform() const {
        return this->transform;
}
}  // namespace planet
