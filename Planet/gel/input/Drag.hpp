#pragma once
#ifndef GEL_INPUT_DRAG_HPP
#define GEL_INPUT_DRAG_HPP
#include <glm/glm.hpp>
#include "Input.hpp"

namespace gel {
class Drag {
       public:
        explicit Drag(MouseButton button);
        void update();
        glm::vec2 getOldPosition() const;
        glm::vec2 getNewPosition() const;
        glm::vec2 getDifference() const;
        bool isDrag() const;

       private:
        MouseButton button;
        bool clicked;
        glm::vec2 oldPosition;
        glm::vec2 newPosition;
};
}  // namespace gel
#endif