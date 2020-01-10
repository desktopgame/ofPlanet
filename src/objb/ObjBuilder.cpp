#include "ObjBuilder.hpp"
#include <sstream>

namespace objb {

// ObjIndex
ObjIndex::ObjIndex(int index) : index(index), valid(true) {
}
ObjIndex::ObjIndex() : index(0), valid(false) {
}
// ObjPolygon
ObjPolygon::ObjPolygon(ObjIndex vertexIndex, ObjIndex texcoordIndex, ObjIndex normalIndex)
	: vertexIndex(vertexIndex), texcoordIndex(texcoordIndex), normalIndex(normalIndex) {
}
ObjPolygon::ObjPolygon(ObjIndex vertexIndex, ObjIndex texcoordIndex)
	: vertexIndex(vertexIndex), texcoordIndex(texcoordIndex), normalIndex() {
}
// ObjModel
ObjModel::ObjModel(const std::string& name) : name(name), vertices(), normals(), texcoords() {
}
ObjModel & ObjModel::vertex(const glm::vec3 & vertex) {
	vertices.emplace_back(vertex);
	return *this;
}
ObjModel & ObjModel::normal(const glm::vec3 & normal) {
	normals.emplace_back(normal);
	return *this;
}
ObjModel & ObjModel::texcoord(const glm::vec2 & texcoord) {
	texcoords.emplace_back(texcoord);
	return *this;
}
ObjModel & ObjModel::face(const ObjFace & face) {
	faces.emplace_back(face);
	return *this;
}
ObjModel & ObjModel::useMaterial(const std::string& material) {
	this->material = material;
	return *this;
}
// ObjBuilder
ObjBuilder::ObjBuilder() : models() {
}
ObjBuilder::~ObjBuilder() {
	for (auto model : models) {
		delete model;
	}
	models.clear();
}
ObjModel& ObjBuilder::newModel(const std::string& name) {
	auto model = new ObjModel(name);
	models.emplace_back(model);
	return *model;
}
ObjBuilder & ObjBuilder::material(const std::string & _material) {
	this->_material = _material;
	return *this;
}
std::string ObjBuilder::toString() const {
	std::stringstream ss;
	write(ss);
	return ss.str();
}
int ObjBuilder::resolveVertexIndex(int modelIndex, int localVertexIndex) const {
	int verts = 0;
	auto model = models.at(modelIndex);
	for (int i = 0; i < modelIndex; i++) {
		verts += static_cast<int>(models.at(i)->vertices.size());
	}
	if (localVertexIndex == -1) {
		return -1;
	}
	return verts + localVertexIndex;
}
int ObjBuilder::resolveNormalIndex(int modelIndex, int localNormalIndex) const {
	int norms = 0;
	auto model = models.at(modelIndex);
	for (int i = 0; i < modelIndex; i++) {
		norms += static_cast<int>(models.at(i)->normals.size());
	}
	if (localNormalIndex == -1) {
		return -1;
	}
	return norms + localNormalIndex;
}
int ObjBuilder::resolveTexcoordIndex(int modelIndex, int localTexcoordIndex) const {
	int texcoords = 0;
	auto model = models.at(modelIndex);
	for (int i = 0; i < modelIndex; i++) {
		texcoords += static_cast<int>(models.at(i)->normals.size());
	}
	if (localTexcoordIndex == -1) {
		return -1;
	}
	return texcoords + localTexcoordIndex;
}
}