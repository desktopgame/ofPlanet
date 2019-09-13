#include "Input.hpp"
#include "../Game.hpp"
#include "../gli.hpp"

namespace gel {
glm::vec2 Input::getMousePosition() {
        auto wnd = gel::Game::getInstance()->getWindow();
        double x, y;
        glfwGetCursorPos(wnd, &x, &y);
        return glm::vec2((float)x, (float)y);
}

ButtonState Input::getMouseState(MouseButton button) {
        auto wnd = gel::Game::getInstance()->getWindow();
        int status = 0;
        switch (button) {
                case gel::MouseButton::Left:
                        status =
                            glfwGetMouseButton(wnd, GLFW_MOUSE_BUTTON_LEFT);
                        break;
                case gel::MouseButton::Right:
                        status =
                            glfwGetMouseButton(wnd, GLFW_MOUSE_BUTTON_RIGHT);
                        break;
                case gel::MouseButton::Middle:
                        status =
                            glfwGetMouseButton(wnd, GLFW_MOUSE_BUTTON_MIDDLE);
                        break;
                default:
                        break;
        }
        if (status == GLFW_PRESS) {
                return ButtonState::Press;
        } else if (status == GLFW_RELEASE) {
                return ButtonState::Release;
        }
        throw std::logic_error("invalid state.");
}
bool Input::isMousePress(MouseButton button) {
        return getMouseState(button) == ButtonState::Press;
}
bool Input::isMouseRelease(MouseButton button) {
        return getMouseState(button) == ButtonState::Release;
}

KeyState Input::getKeyState(int keyCode) {
        auto wnd = gel::Game::getInstance()->getWindow();
        int status = glfwGetKey(wnd, keyCode);
        if (status == GLFW_PRESS) {
                return KeyState::Press;
        } else if (status == GLFW_RELEASE) {
                return KeyState::Release;
        }
        throw std::logic_error("invalid state.");
}
bool Input::isKeyPress(int keyCode) {
        return getKeyState(keyCode) == KeyState::Press;
}
bool Input::isKeyRelease(int keyCode) {
        return getKeyState(keyCode) == KeyState::Release;
}
}  // namespace gel