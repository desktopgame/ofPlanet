#pragma once
#ifndef COMMON_MOUSESCROLL_HPP
#define COMMON_MOUSESCROLL_HPP
#include <ofVec3f.h>
namespace planet {

class MouseScroll {
       public:
        explicit MouseScroll();
        void enable();
        void disable();
        void update();

        glm::vec2 getAmount() const;

       private:
        bool enabled;
        glm::vec2 oldMousePos;
        glm::vec2 diffMousePos;
};
}  // namespace planet
#endif