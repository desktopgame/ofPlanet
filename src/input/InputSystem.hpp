#pragma once
#ifndef INPUT_INPUTSYSTEM_HPP
#define INPUT_INPUTSYSTEM_HPP
#include "InputState.hpp"
namespace planet {
class InputSystem {
       public:
        explicit InputSystem();
        void updateBuffers();
        std::shared_ptr<MouseState> getMouseState() const;
        std::shared_ptr<KeyboardState> getKeyboardState() const;

       private:
        std::shared_ptr<InputState> inputState;
};
}  // namespace planet
#endif