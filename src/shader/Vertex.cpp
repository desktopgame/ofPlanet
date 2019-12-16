#include "Vertex.hpp"
namespace planet {

Vertex::Vertex() : vertex(0, 0, 0), normal(0, 0, 0), uv(0, 0) {}
Vertex::Vertex(glm::vec3 vertex, glm::vec3 normal, glm::vec2 uv)
    : vertex(vertex), normal(normal), uv(uv) {}
}  // namespace planet
