#pragma once
#ifndef SHADER_TRANSFORM_HPP
#define SHADER_TRANSFORM_HPP
#include <ofVec3f.h>
namespace planet {
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
}  // namespace planet
#endif