#pragma once
#ifndef INPUT_MOUSESTATE_HPP
#define INPUT_MOUSESTATE_HPP
#include "ButtonState.hpp"
#include "MouseBuffer.hpp"
#include "MouseState.hpp"
namespace planet {

class MouseState {
       public:
        explicit MouseState(const MouseBuffer& buffer);
        bool getButtonValue(MouseButton button) const;
        ButtonState getButtonState(MouseButton button) const;
        glm::vec2 getPosition() const;
        glm::vec2 getScrollWheel() const;

       private:
        const MouseBuffer& buffer;
};
}  // namespace planet
#endif