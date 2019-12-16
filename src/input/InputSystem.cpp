#include "InputSystem.hpp"
namespace planet {

InputSystem::InputSystem() : inputState(std::make_shared<InputState>()) {}

void InputSystem::updateBuffers() { inputState->updateBuffers(); }

std::shared_ptr<MouseState> InputSystem::getMouseState() const {
        return inputState->getMouseState();
}

std::shared_ptr<KeyboardState> InputSystem::getKeyboardState() const {
        return inputState->getKeyboardState();
}
}  // namespace planet
