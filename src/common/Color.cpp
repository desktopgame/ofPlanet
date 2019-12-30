#include "Color.hpp"
namespace planet {
Color::Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {
}
Color::Color(float r, float g, float b) : Color(r, g, b, 1.0f) {
}
}