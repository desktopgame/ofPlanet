#pragma once
#ifndef INPUT_INPUTSTATE_HPP
#define INPUT_INPUTSTATE_HPP
#include <memory>

#include "KeyboardBuffer.hpp"
#include "MouseBuffer.hpp"
namespace planet {
class MouseState;
class KeyboardState;

class InputState {
       public:
        explicit InputState();
        void updateBuffers();
        std::shared_ptr<MouseState> getMouseState() const;
        std::shared_ptr<KeyboardState> getKeyboardState() const;

       private:
        MouseBuffer mouseBuffer;
        std::shared_ptr<MouseState> mouseState;
        KeyboardBuffer keyboardBuffer;
        std::shared_ptr<KeyboardState> keyboardState;
};
}  // namespace planet
#endif