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
	
	template<typename StreamType>
	void write(StreamType& stream) const;
private:
	template<typename StreamType>
	void writeImpl(StreamType& stream, int index) const;

	std::string _material;
	int resolveVertexIndex(int modelIndex, int localVertexIndex) const;
	int resolveNormalIndex(int modelIndex, int localNormalIndex) const;
	int resolveTexcoordIndex(int modelIndex, int localTexcoordIndex) const;
	std::vector<ObjModel*> models;
};
template<typename StreamType>
inline void ObjBuilder::write(StreamType & stream) const {
	if (!_material.empty()) {
		stream << "mtllib " << _material << std::endl;
	}
	for (int i = 0; i < static_cast<int>(models.size()); i++) {
		writeImpl(stream, i);
	}
}
template<typename StreamType>
inline void ObjBuilder::writeImpl(StreamType & stream, int index) const {
	auto model = models.at(index);
	stream << "o " << model->name << std::endl;
	for (auto vert : model->vertices) {
		stream << "v " << vert.x << " " << vert.y << " " << vert.z << std::endl;
	}
	for (auto norm : model->normals) {
		stream << "vn " << norm.x << " " << norm.y << " " << norm.z << std::endl;
	}
	for (auto texcoord : model->texcoords) {
		stream << "vt " << texcoord.x << " " << texcoord.y << std::endl;
	}
	for (auto face : model->faces) {
		stream << "f ";
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
				stream << vi << "/" << ti << "/" << ni;
			}
			else if (polygon.vertexIndex.valid && polygon.texcoordIndex.valid) {
				stream << vi << "/" << ti;
			}
			else if (polygon.vertexIndex.valid) {
				stream << vi;
			}
			if (!last) {
				stream << " ";
			}
		}
		if (!model->material.empty()) {
			stream << "usemtl " << model->material << std::endl;
		}
		stream << std::endl;
	}
}
}
#endif