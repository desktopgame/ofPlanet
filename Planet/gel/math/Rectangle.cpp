#include "Rectangle.hpp"

namespace gel {
Rectangle::Rectangle() : x(0), y(0), width(0), height(0) {}
Rectangle::Rectangle(float x, float y, float width, float height)
    : x(x), y(y), width(width), height(height) {}
Rectangle::Rectangle(const glm::vec2 pos, const glm::vec2 size)
    : x(pos.x), y(pos.y), width(size.x), height(size.y) {}
float Rectangle::left() const { return x; }
float Rectangle::top() const { return y; }
float Rectangle::right() const { return x + width; }
float Rectangle::bottom() const { return y + height; }
bool Rectangle::contains(float x, float y) const {
        return (x > left() && x < right()) && (y > top() && y < bottom());
}
bool Rectangle::isIntersects(const Rectangle a, const Rectangle b) {
        return !(b.left() > a.right() || b.right() < a.left() ||
                 b.top() > a.bottom() || b.bottom() < a.top());
}
}  // namespace gel