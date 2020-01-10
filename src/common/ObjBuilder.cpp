#include "ObjBuilder.hpp"
#include <sstream>

namespace planet {
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
	if (!_material.empty()) {
		ss << "mtllib " << _material << std::endl;
	}
	for (int i = 0; i < static_cast<int>(models.size()); i++) {
		toStringImpl(ss, i);
	}
	return ss.str();
}
void ObjBuilder::toStringImpl(std::stringstream & ss, int index) const {
	auto model = models.at(index);
	ss << "o " << model->name << std::endl;
	for (auto vert : model->vertices) {
		ss << "v " << vert.x << " " << vert.y << " " << vert.z << std::endl;
	}
	for (auto norm : model->normals) {
		ss << "vn " << norm.x << " " << norm.y << " " << norm.z << std::endl;
	}
	for (auto texcoord : model->texcoords) {
		ss << "vt " << texcoord.x << " " << texcoord.y << std::endl;
	}
	for (auto face : model->faces) {
		ss << "f ";
		for (int i = 0; i < static_cast<int>(face.size()); i++) {
			bool last = i == (face.size() - 1);
			auto polygon = face[i];
			int vi = polygon.vertexIndex.index;
			int ti = polygon.texcoordIndex.index;
			int ni = polygon.normalIndex.index;
			vi = resolveVertexIndex(index, vi);
			ti = resolveTexcoordIndex(index, ti);
			ni = resolveNormalIndex(index, ni);
			if (polygon.vertexIndex.valid && polygon.texcoordIndex.valid && polygon.normalIndex.valid) {
				ss << vi << "/" << ti << "/" << ni;
			}
			else if (polygon.vertexIndex.valid && polygon.texcoordIndex.valid) {
				ss << vi << "/" << ti;
			}
			else if (polygon.vertexIndex.valid) {
				ss << vi;
			}
			if (!last) {
				ss << " ";
			}
		}
		if (!model->material.empty()) {
			ss << "usemtl " << model->material << std::endl;
		}
		ss << std::endl;
	}
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