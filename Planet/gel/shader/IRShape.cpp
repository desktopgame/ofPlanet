#include "IRShape.hpp"
namespace gel {
IRShape::IRShape() : vertexBuf(), normalBuf(), uvBuf(), indexBuf() {}

IRShape::~IRShape() {}

// vertex
void IRShape::addVertex(const glm::vec4 vertex) { vertexBuf.push_back(vertex); }

glm::vec4 IRShape::getVertexAt(int index) const { return vertexBuf.at(index); }

int IRShape::getVertexCount() const { return vertexBuf.size(); }
// uv
void IRShape::addUV(const glm::vec2 uv) { uvBuf.push_back(uv); }
glm::vec2 IRShape::getUVAt(int index) const { return uvBuf.at(index); }

int IRShape::getUVCount() const { return uvBuf.size(); }
// normal
void IRShape::addNormal(const glm::vec4 normal) { normalBuf.push_back(normal); }
glm::vec4 IRShape::getNormalAt(int index) const { return normalBuf.at(index); }

int IRShape::getNormalCount() const { return normalBuf.size(); }
// index
void IRShape::addIndex(int index) { indexBuf.push_back(index); }

int IRShape::getIndexAt(int index) const { return indexBuf.at(index); }

int IRShape::getIndexCount() const { return indexBuf.size(); }

}  // namespace gel