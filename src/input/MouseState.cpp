#include "MouseState.hpp"
namespace planet {

MouseState::MouseState(const MouseBuffer& buffer) : buffer(buffer) {}

bool MouseState::getButtonValue(MouseButton button) const {
        return buffer.currButton[static_cast<unsigned int>(button)];
}

ButtonState MouseState::getButtonState(MouseButton button) const {
        bool prev = buffer.prevButton[static_cast<unsigned int>(button)];
        bool curr = buffer.currButton[static_cast<unsigned int>(button)];
        if (!prev) {
                if (curr) {
                        return ButtonState::Press;
                } else {
                        return ButtonState::None;
                }
        } else {
                if (curr) {
                        return ButtonState::Held;
                } else {
                        return ButtonState::Release;
                }
        }
}

glm::vec2 MouseState::getPosition() const { return buffer.position; }

glm::vec2 MouseState::getScrollWheel() const { return buffer.scrollWheel; }
}  // namespace planet
