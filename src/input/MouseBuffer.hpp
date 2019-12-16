#pragma once
#ifndef INPUT_MOUSEBUFFER_HPP
#define INPUT_MOUSEBUFFER_HPP
#include "../common/GLM.hpp"

#include "MouseButton.hpp"
namespace planet {

class MouseBuffer {
       public:
        explicit MouseBuffer();
        bool prevButton[static_cast<unsigned int>(MouseButton::Count)];
        bool currButton[static_cast<unsigned int>(MouseButton::Count)];
        glm::vec2 position;
        glm::vec2 scrollWheel;
};
}  // namespace planet
#endif