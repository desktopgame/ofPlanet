#include "MouseTrigger.hpp"
#include "../Game.hpp"
#include "../gli.hpp"
namespace gel {
MouseTrigger::MouseTrigger(const GLenum mode)
    : mode(mode), triggered(false), enabled(false) {}
void MouseTrigger::update() {
        GLFWwindow* w = Game::getInstance()->getWindow();
        if (triggered) {
                if (glfwGetMouseButton(w, mode) == GLFW_PRESS) {
                        this->enabled = false;
                } else if (glfwGetMouseButton(w, mode) == GLFW_RELEASE) {
                        this->triggered = false;
                        this->enabled = false;
                }
        } else {
                if (glfwGetMouseButton(w, mode) == GLFW_PRESS) {
                        this->triggered = true;
                        this->enabled = true;
                }
        }
}
bool MouseTrigger::isEnabled() const { return enabled; }
}  // namespace gel