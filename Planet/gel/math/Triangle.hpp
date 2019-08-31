#ifndef GEL_MATH_TRIANGLE_HPP
#define GEL_MATH_TRIANGLE_HPP
#include <glm/glm.hpp>
namespace gel {
struct Triangle {
        Triangle();
        glm::vec4 ver;
        glm::vec4 nor;
        glm::vec2 uv;
};
}  // namespace  gel
#endif