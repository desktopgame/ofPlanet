#include "Input.hpp"

#include "../game/ofApp.h"
namespace planet {

glm::vec2 Input::getMousePosition() {
        return ofApp::getMouseState()->getPosition();
}

ButtonState Input::getMouseState(MouseButton button) {
        return ofApp::getMouseState()->getButtonState(button);
}

bool Input::isMousePress(MouseButton button) {
        return getMouseState(button) == ButtonState::Press;
}

bool Input::isMouseRelease(MouseButton button) {
        return getMouseState(button) == ButtonState::Release;
}

ButtonState Input::getKeyState(int keyCode) {
        return ofApp::getKeyboardState()->getKeyState(keyCode);
}

bool Input::getKey(int keyCode) {
	auto stat = getKeyState(keyCode);
	return stat == ButtonState::Press || stat == ButtonState::Held;
}

bool Input::isKeyPress(int keyCode) {
        return getKeyState(keyCode) == ButtonState::Press;
}

bool Input::isKeyRelease(int keyCode) {
        return getKeyState(keyCode) == ButtonState::Release;
}
}  // namespace planet
