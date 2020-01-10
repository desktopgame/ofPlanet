#pragma once
#ifndef OBJB_OBJBUILDER_HPP
#define OBJB_OBJBUILDER_HPP
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <ostream>

namespace objb {
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
class ObjModel {
public:
	explicit ObjModel(const std::string& name);
	ObjModel& vertex(const glm::vec3& vertex);
	ObjModel& normal(const glm::vec3& normal);
	ObjModel& texcoord(const glm::vec2& texcoord);
	ObjModel& face(const ObjFace& face);
	ObjModel& useMaterial(const std::string& material);
	std::string name;
	std::string material;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texcoords;
	std::vector<ObjFace> faces;
};
class ObjBuilder {
public:
	explicit ObjBuilder();
	~ObjBuilder();
	ObjModel& newModel(const std::string& name);
	ObjBuilder& material(const std::string& _material);
	std::string toString() const;
private:
	std::string _material;
	void toStringImpl(std::stringstream& ss, int index) const;
	int resolveVertexIndex(int modelIndex, int localVertexIndex) const;
	int resolveNormalIndex(int modelIndex, int localNormalIndex) const;
	int resolveTexcoordIndex(int modelIndex, int localTexcoordIndex) const;
	std::vector<ObjModel*> models;
};
}
#endif