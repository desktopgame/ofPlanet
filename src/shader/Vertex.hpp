#pragma once
#ifndef SHADER_VERTEX_HPP
#define SHADER_VERTEX_HPP
#include "../common/GLM.hpp"
namespace planet {

struct Vertex {
        explicit Vertex();
        explicit Vertex(glm::vec3 vertex, glm::vec3 normal, glm::vec2 uv);
        glm::vec3 vertex;
        glm::vec3 normal;
        glm::vec2 uv;
};
}  // namespace planet
#endif