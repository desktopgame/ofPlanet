#include "KeyTrigger.hpp"
#include "../Game.hpp"
#include "../gli.hpp"
namespace gel {
KeyTrigger::KeyTrigger(const int key)
    : key(key), triggered(false), enabled(false) {}

void KeyTrigger::update() {
        GLFWwindow* w = Game::getInstance()->getWindow();
        if (triggered) {
                if (glfwGetKey(w, key) == GLFW_PRESS) {
                        this->enabled = false;
                } else if (glfwGetKey(w, key) == GLFW_RELEASE) {
                        this->triggered = false;
                }
        } else {
                if (glfwGetKey(w, key) == GLFW_PRESS) {
                        this->triggered = true;
                        this->enabled = true;
                }
        }
}
bool KeyTrigger::isEnabled() const { return enabled; }
}  // namespace gel