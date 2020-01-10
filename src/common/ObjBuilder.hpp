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
class ObjModel {
public:
	explicit ObjModel(const std::string& name);
	ObjModel& vertex(const glm::vec3& vertex);
	ObjModel& normal(const glm::vec3& normal);
	ObjModel& texcoord(const glm::vec2& texcoord);
	ObjModel& face(const ObjFace& face);
	std::string name;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texcoords;
	std::vector<ObjFace> faces;
};
std::stringstream& operator<< (std::stringstream& ss, const ObjModel& model);
std::stringstream& operator<< (std::stringstream& ss, const ObjModel* model);
class ObjBuilder {
public:
	explicit ObjBuilder();
	~ObjBuilder();
	ObjModel& newModel(const std::string& name);
	std::string toString() const;
private:
	std::vector<ObjModel*> models;
};
}
#endif