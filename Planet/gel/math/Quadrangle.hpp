#ifndef GEL_MATH_QUADRANGLE_HPP
#define GEL_MATH_QUADRANGLE_HPP
#include <glm/glm.hpp>
namespace gel {
struct Quadrangle {
        Quadrangle();
        glm::vec4 ver;
        glm::vec4 nor;
        glm::vec2 uv;
};
}  // namespace gel
#endif