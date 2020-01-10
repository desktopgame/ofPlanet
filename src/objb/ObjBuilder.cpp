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
void ObjBuilder::reserveModels(int size) {
	models.reserve(size);
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
int ObjBuilder::countVertex(std::vector<int>& cache, int modelIndex) const {
	int verts = -1;
	if (modelIndex < static_cast<int>(cache.size())) {
		verts = cache.at(modelIndex);
	}
	if (verts == -1) {
		if (modelIndex == 0) {
			verts = static_cast<int>(models.at(0)->vertices.size());
		} else {
			verts = countVertex(cache, modelIndex - 1);
			verts += models[modelIndex]->vertices.size();
		}
		while (static_cast<int>(cache.size()) <= modelIndex) {
			cache.emplace_back(-1);
		}
		cache[modelIndex] = verts;
	}
	return verts;
}
int ObjBuilder::countNormal(std::vector<int>& cache, int modelIndex) const {
	int norms = -1;
	if (modelIndex < static_cast<int>(cache.size())) {
		norms = cache.at(modelIndex);
	}
	if (norms == -1) {
		if (modelIndex == 0) {
			norms = static_cast<int>(models.at(0)->normals.size());
		}
		else {
			norms = countVertex(cache, modelIndex - 1);
			norms += models[modelIndex]->normals.size();
		}
		while (static_cast<int>(cache.size()) <= modelIndex) {
			cache.emplace_back(-1);
		}
		cache[modelIndex] = norms;
	}
	return norms;
}
int ObjBuilder::countTexcoord(std::vector<int>& cache, int modelIndex) const {
	int texcoords = -1;
	if (modelIndex < static_cast<int>(cache.size())) {
		texcoords = cache.at(modelIndex);
	}
	if (texcoords == -1) {
		if (modelIndex == 0) {
			texcoords = static_cast<int>(models.at(0)->texcoords.size());
		}
		else {
			texcoords = countVertex(cache, modelIndex - 1);
			texcoords += models[modelIndex]->texcoords.size();
		}
		while (static_cast<int>(cache.size()) <= modelIndex) {
			cache.emplace_back(-1);
		}
		cache[modelIndex] = texcoords;
	}
	return texcoords;
}
int ObjBuilder::resolveVertexIndex(std::vector<int>& cache, int modelIndex, int localVertexIndex) const {
	if (localVertexIndex == -1) {
		return -1;
	}
	return countVertex(cache, modelIndex) - static_cast<int>(models[modelIndex]->vertices.size()) + localVertexIndex;
}
int ObjBuilder::resolveNormalIndex(std::vector<int>& cache, int modelIndex, int localNormalIndex) const {
	return countNormal(cache, modelIndex) - static_cast<int>(models[modelIndex]->normals.size()) + localNormalIndex;
}
int ObjBuilder::resolveTexcoordIndex(std::vector<int>& cache, int modelIndex, int localTexcoordIndex) const {

	return countTexcoord(cache, modelIndex) - static_cast<int>(models[modelIndex]->texcoords.size()) + localTexcoordIndex;
}
}