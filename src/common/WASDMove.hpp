#pragma once
#ifndef COMMON_WASDMOVE_HPP
#define COMMON_WASDMOVE_HPP
#include <ofVec3f.h>
namespace planet {

class WASDMove {
       public:
        explicit WASDMove();

        void enable();
        void disable();
        void update();

        float getScale() const;
        glm::ivec2 getAxis() const;
        bool isSprint() const;

       private:
        bool enabled;
        bool sprint;
        glm::ivec2 axis;
};
}  // namespace planet
#endif