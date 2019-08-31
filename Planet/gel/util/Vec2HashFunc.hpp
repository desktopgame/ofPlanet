#ifndef GEL_UTIL_VEC2HASHFUNC_HPP
#define GEL_UTIL_VEC2HASHFUNC_HPP
#include <glm/glm.hpp>
namespace gel {

struct Vec2HashFunc {
        size_t operator()(const glm::ivec2& k) const {
                return std::hash<int>()(k.x) ^ std::hash<int>()(k.y);
        }

        bool operator()(const glm::ivec2& a, const glm::ivec2& b) const {
                return a.x == b.x && a.y == b.y;
        }
};
}  // namespace gel
#endif