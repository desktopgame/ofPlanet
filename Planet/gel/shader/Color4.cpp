#include "Color4.hpp"
namespace gel {
Color4::Color4() : r(0), g(0), b(0), a(1) {}

Color4::Color4(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}

Color4 Color4::white(float a) { return Color4(1, 1, 1, a); }

Color4 Color4::black(float a) { return Color4(0, 0, 0, a); }
}  // namespace gel
