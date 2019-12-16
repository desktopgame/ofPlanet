#include "KeyboardState.hpp"
namespace planet {

KeyboardState::KeyboardState(const KeyboardBuffer& buffer) : buffer(buffer) {}

bool KeyboardState::getKeyValue(int key) const {
        return buffer.currKeymap.at(key);
}

ButtonState KeyboardState::getKeyState(int key) const {
        if (!buffer.prevKeymap.at(key)) {
                if (buffer.currKeymap.at(key)) {
                        return ButtonState::Press;
                } else {
                        return ButtonState::None;
                }
        } else {
                if (buffer.currKeymap.at(key)) {
                        return ButtonState::Held;
                } else {
                        return ButtonState::Release;
                }
        }
}
}  // namespace planet
