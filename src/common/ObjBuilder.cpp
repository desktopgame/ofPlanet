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
// ObjBuilder
ObjBuilder::ObjBuilder() : vertices(), normals(), texcoords() {
}
ObjBuilder & ObjBuilder::vertex(const glm::vec3 & vertex) {
	vertices.emplace_back(vertex);
	return *this;
}
ObjBuilder & ObjBuilder::normal(const glm::vec3 & normal) {
	normals.emplace_back(normal);
	return *this;
}
ObjBuilder & ObjBuilder::texcoord(const glm::vec2 & texcoord) {
	texcoords.emplace_back(texcoord);
	return *this;
}
ObjBuilder & ObjBuilder::face(const ObjFace & face) {
	faces.emplace_back(face);
	return *this;
}
std::string ObjBuilder::toString() const {
	std::stringstream ss;
	for (auto vert : vertices) {
		ss << "v " << vert.x << " " << vert.y << " " << vert.z << std::endl;
	}
	for (auto norm : normals) {
		ss << "vn " << norm.x << " " << norm.y << " " << norm.z << std::endl;
	}
	for (auto texcoord : texcoords) {
		ss << "vt " << texcoord.x << " " << texcoord.y << std::endl;
	}
	for (auto face : faces) {
		ss << "f ";
		for (int i = 0; i < static_cast<int>(face.size()); i++) {
			bool last = i == (face.size() - 1);
			auto polygon = face[i];
			int vi = polygon.vertexIndex.index;
			int ti = polygon.texcoordIndex.index;
			int ni = polygon.normalIndex.index;
			if (polygon.vertexIndex.valid && polygon.texcoordIndex.valid && polygon.normalIndex.valid) {
				ss << vi << "/" << ti << "/" << ni;
			} else if (polygon.vertexIndex.valid && polygon.texcoordIndex.valid) {
				ss << vi << "/" << ti;
			} else if (polygon.vertexIndex.valid) {
				ss << vi;
			}
			if (!last) {
				ss << " ";
			}
		}
		ss << std::endl;
	}
	return ss.str();
}
}