#pragma once
#ifndef OBJB_OBJBUILDER_HPP
#define OBJB_OBJBUILDER_HPP
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <ostream>

namespace objb {
enum class IndexMode {
	Local,
	Global,
};
struct ObjIndex {
	int index;
	bool valid;
	IndexMode mode;

	explicit ObjIndex(int index, IndexMode mode);
	explicit ObjIndex(int index);
	explicit ObjIndex();
};
struct ObjPolygon {
	ObjIndex vertexIndex;
	ObjIndex texcoordIndex;
	ObjIndex normalIndex;

	explicit ObjPolygon(ObjIndex vertexIndex, ObjIndex texcoordIndex, ObjIndex normalIndex);
	explicit ObjPolygon(ObjIndex vertexIndex, ObjIndex texcoordIndex);
};
using ObjFace = std::vector<ObjPolygon>;
class ObjBuilder;
class ObjModel {
public:
	explicit ObjModel(ObjBuilder& builder, const std::string& name);
	ObjModel& sharedVertex(const glm::vec3& aVertex, ObjPolygon& destPoly);
	ObjModel& vertex(const glm::vec3& vertex);
	ObjModel& normal(const glm::vec3& normal);
	ObjModel& texcoord(const glm::vec2& texcoord);
	ObjModel& face(const ObjFace& face);
	ObjModel& useMaterial(const std::string& material);
	int getUseIndexCount();
	std::string name;
	std::string material;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texcoords;
	std::vector<ObjFace> faces;
private:
	int useIndexCount;
	ObjBuilder& builder;
};
class ObjBuilder {
public:
	explicit ObjBuilder();
	~ObjBuilder();
	ObjModel& newModel(const std::string& name);
	void reserveModels(int size);
	ObjBuilder& material(const std::string& _material);
	ObjBuilder& globalVertex(glm::vec3 vertex);
	ObjBuilder& globalNormal(glm::vec3 normal);
	ObjBuilder& globalTexcoord(glm::vec2 texcoord);
	std::string toString() const;
	
	template<typename StreamType>
	void write(StreamType& stream) const;

	ObjModel& getModelAt(int index);
	int getModelCount() const;
	int getGlobalVertexCount() const;
	int getGlobalNormalCount() const;
	int getGloalTexcoordCount() const;

	int countVertex();
private:
	template<typename StreamType>
	void writeImpl(StreamType& stream, int index, std::vector<int>& vertCache, std::vector<int>& normCache, std::vector<int>& texcoordCache) const;

	std::string _material;
	int countVertex(std::vector<int>& cache, int modelIndex) const;
	int countNormal(std::vector<int>& cache, int modelIndex) const;
	int countTexcoord(std::vector<int>& cache, int modelIndex) const;
	int resolveVertexIndex(std::vector<int>& cache, int modelIndex, int localVertexIndex) const;
	int resolveNormalIndex(std::vector<int>& cache, int modelIndex, int localNormalIndex) const;
	int resolveTexcoordIndex(std::vector<int>& cache, int modelIndex, int localTexcoordIndex) const;
	std::vector<ObjModel*> models;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texcoords;
	int allVertexCount;
};
template<typename StreamType>
inline void ObjBuilder::write(StreamType & stream) const {
	if (!_material.empty()) {
		stream << "mtllib " << _material << std::endl;
	}
	std::vector<int> vcache, ncache, tccache;
	int size = static_cast<int>(models.size());
	stream << "# Global Data" << std::endl;
	for (auto vert : vertices) {
		stream << "v " << vert.x << " " << vert.y << " " << vert.z << std::endl;
	}
	for (auto norm : normals) {
		stream << "vn " << norm.x << " " << norm.y << " " << norm.z << std::endl;
	}
	for (auto texcoord : texcoords) {
		stream << "vt " << texcoord.x << " " << texcoord.y << std::endl;
	}
	stream << "# Local Data" << std::endl;
	// æ‚É’¸“_, –@ü, UV‚¾‚¯‚ð‘‚«ž‚ñ‚Å‚¨‚­
	for (int i = 0; i < size; i++) {
		auto model = models.at(i);
		for (auto vert : model->vertices) {
			stream << "v " << vert.x << " " << vert.y << " " << vert.z << std::endl;
		}
		for (auto norm : model->normals) {
			stream << "vn " << norm.x << " " << norm.y << " " << norm.z << std::endl;
		}
		for (auto texcoord : model->texcoords) {
			stream << "vt " << texcoord.x << " " << texcoord.y << std::endl;
		}
	}
	// –Ê‚ðì¬‚·‚é
	for (int i = 0; i <size; i++) {
		writeImpl(stream, i, vcache, ncache, tccache);
	}
}
template<typename StreamType>
inline void ObjBuilder::writeImpl(StreamType & stream, int index, std::vector<int>& vertCache, std::vector<int>& normCache, std::vector<int>& texcoordCache) const {
	auto model = models.at(index);
	for (auto face : model->faces) {
		stream << "o " << model->name << std::endl;
		if (!model->material.empty()) {
			stream << "usemtl " << model->material << std::endl;
		}
		stream << "f ";
		for (int i = 0; i < static_cast<int>(face.size()); i++) {
			bool last = i == (face.size() - 1);
			auto polygon = face[i];
			int vi = polygon.vertexIndex.index;
			int ti = polygon.texcoordIndex.index;
			int ni = polygon.normalIndex.index;
			if (polygon.vertexIndex.mode == IndexMode::Local) {
				vi = resolveVertexIndex(vertCache, index, vi);
			}
			if (polygon.normalIndex.mode == IndexMode::Local) {
				ni = resolveNormalIndex(normCache, index, ni);
			}
			if (polygon.texcoordIndex.mode == IndexMode::Local) {
				ti = resolveTexcoordIndex(texcoordCache, index, ti);
			}
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
		stream << std::endl;
	}
}
}
#endif