#ifndef GEL_GSYSTEM_TRANSFORM_HPP
#define GEL_GSYSTEM_TRANSFORM_HPP
#include <glm/glm.hpp>
namespace gel {
struct Transform {
        Transform();
        glm::vec3 forward() const;
        glm::vec3 backward() const;
        glm::vec3 left() const;
        glm::vec3 right() const;

        glm::vec3 position;
        glm::vec3 scale;
        glm::vec3 rotation;
};
}  // namespace gel
#endif