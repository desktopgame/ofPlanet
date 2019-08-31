#ifndef GAME_MATH_RECTANGLE_HPP
#define GAME_MATH_RECTANGLE_HPP
#include <glm/glm.hpp>

namespace gel {
class Rectangle {
       public:
        explicit Rectangle();
        explicit Rectangle(float x, float y, float width, float height);
        explicit Rectangle(const glm::vec2 pos, const glm::vec2 size);
        float left() const;
        float top() const;
        float right() const;
        float bottom() const;
        bool contains(float x, float y) const;
        static bool isIntersects(const Rectangle a, const Rectangle b);

       private:
        float x;
        float y;
        float width;
        float height;
};
}  // namespace gel
#endif