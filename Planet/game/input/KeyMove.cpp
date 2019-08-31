#include "KeyMove.hpp"
KeyMove::KeyMove(const gel::Transform& transform, float moveSpeed,
                 float jumpSpeed)
    : transform(transform), moveSpeed(moveSpeed), jumpSpeed(jumpSpeed) {}

void KeyMove::update(bool isGround) {
        this->force = glm::vec3(0, 0, 0);
        this->_isJumping = false;
        auto mWindow = gel::Game::getInstance()->getWindow();
        auto factor = (glfwGetKey(mWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
                          ? 2.0f
                          : 1.0f;
        if (glfwGetKey(mWindow, 'W') == GLFW_PRESS) {
                this->force = glm::vec3(1, 0, 1) * transform.forward() *
                              moveSpeed * factor;
        } else if (glfwGetKey(mWindow, 'S') == GLFW_PRESS) {
                this->force = glm::vec3(1, 0, 1) * transform.backward() *
                              moveSpeed * factor;
        }
        if (glfwGetKey(mWindow, 'A') == GLFW_PRESS) {
                this->force =
                    glm::vec3(1, 0, 1) * transform.left() * moveSpeed * factor;
        } else if (glfwGetKey(mWindow, 'D') == GLFW_PRESS) {
                this->force =
                    glm::vec3(1, 0, 1) * transform.right() * moveSpeed * factor;
        }
        if (isGround && glfwGetKey(mWindow, GLFW_KEY_SPACE) == GLFW_PRESS) {
                this->force.y = jumpSpeed;
                this->_isJumping = true;
        }
}
glm::vec3 KeyMove::getVelocity() const { return force; }

bool KeyMove::isJumping() const { return _isJumping; }