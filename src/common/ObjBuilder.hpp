#pragma once
#ifndef COMMON_OBJBUILDER_HPP
#define COMMON_OBJBUILDER_HPP
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <ostream>

namespace planet {
struct ObjIndex {
	const int index;
	const bool valid;
	
	explicit ObjIndex(int index);
	explicit ObjIndex();
};
struct ObjPolygon {
	const ObjIndex vertexIndex;
	const ObjIndex texcoordIndex;
	const ObjIndex normalIndex;

	explicit ObjPolygon(ObjIndex vertexIndex, ObjIndex texcoordIndex, ObjIndex normalIndex);
	explicit ObjPolygon(ObjIndex vertexIndex, ObjIndex texcoordIndex);
};
using ObjFace = std::vector<ObjPolygon>;
class ObjBuilder {
public:
	explicit ObjBuilder();

	ObjBuilder& vertex(const glm::vec3& vertex);
	ObjBuilder& normal(const glm::vec3& normal);
	ObjBuilder& texcoord(const glm::vec2& texcoord);
	ObjBuilder& face(const ObjFace& face);
	std::string toString() const;
private:
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texcoords;
	std::vector<ObjFace> faces;
};
}
#endif