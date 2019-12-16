#pragma once
#ifndef INPUT_KEYBOARDSTATE_HPP
#define INPUT_KEYBOARDSTATE_HPP
#include "ButtonState.hpp"
#include "KeyboardBuffer.hpp"
namespace planet {

class KeyboardState {
       public:
        explicit KeyboardState(const KeyboardBuffer& buffer);
        bool getKeyValue(int key) const;
        ButtonState getKeyState(int key) const;

       private:
        const KeyboardBuffer& buffer;
};
}  // namespace planet
#endif