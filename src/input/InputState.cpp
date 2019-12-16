#include "InputState.hpp"

#include "../common/glfw.hpp"
#include "KeyboardState.hpp"
#include "MouseState.hpp"
namespace planet {

InputState::InputState()
    : mouseBuffer(),
      mouseState(std::make_shared<MouseState>(mouseBuffer)),
      keyboardBuffer(),
      keyboardState(std::make_shared<KeyboardState>(keyboardBuffer)) {}

void InputState::updateBuffers() {
        // update KeyboardBuffer
        for (auto kv : keyboardBuffer.currKeymap) {
                keyboardBuffer.prevKeymap.insert_or_assign(kv.first, kv.second);
        }
        for (int key : KeyboardBuffer::GLFW_KEY_LIST) {
                int stat = glfw::getKey(key);
                keyboardBuffer.currKeymap.insert_or_assign(key,
                                                           stat == glfw::Press);
        }
        // update MouseBuffer
        double cx = glfw::getMouseX();
        double cy = glfw::getMouseY();
        mouseBuffer.position =
            glm::ivec2(static_cast<int>(cx), static_cast<int>(cy));
        for (int i = 0; i < 3; i++) {
                mouseBuffer.prevButton[i] = mouseBuffer.currButton[i];
        }
        mouseBuffer.currButton[static_cast<int>(MouseButton::Left)] =
            glfw::getMouseButton(glfw::Mouse_button_left);
        mouseBuffer.currButton[static_cast<int>(MouseButton::Middle)] =
            glfw::getMouseButton(glfw::Mouse_button_middle);
        mouseBuffer.currButton[static_cast<int>(MouseButton::Right)] =
            glfw::getMouseButton(glfw::Mouse_button_right);
}

std::shared_ptr<MouseState> InputState::getMouseState() const {
        return mouseState;
}

std::shared_ptr<KeyboardState> InputState::getKeyboardState() const {
        return keyboardState;
}
}  // namespace planet
