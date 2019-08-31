#include "FbxMeshInfo.hpp"
namespace gel {
FbxMeshInfo::FbxMeshInfo()
    : textureBuf(),
      vertexBuf(),
      normalBuf(),
      materialBuf(),
      uvBuf(),
      indexBuf(),
      texIdBuf(),
      children(),
      parent() {}

void FbxMeshInfo::addChild(const std::shared_ptr<FbxMeshInfo> info) {
        children.push_back(info);
        info->parent = shared_from_this();
}

std::shared_ptr<FbxMeshInfo> FbxMeshInfo::getChildAt(int index) const {
        return children[index];
}

int FbxMeshInfo::getChildCount() const {
        return static_cast<int>(children.size());
}

// vertex
void FbxMeshInfo::addVertex(const glm::vec4 vertex) {
        vertexBuf.push_back(vertex);
}

glm::vec4 FbxMeshInfo::getVertexAt(int index) const {
        return vertexBuf.at(index);
}

int FbxMeshInfo::getVertexCount() const { return vertexBuf.size(); }
// uv
void FbxMeshInfo::addUV(const glm::vec2 uv) { uvBuf.push_back(uv); }
glm::vec2 FbxMeshInfo::getUVAt(int index) const { return uvBuf.at(index); }

int FbxMeshInfo::getUVCount() const { return uvBuf.size(); }
// normal
void FbxMeshInfo::addNormal(const glm::vec4 normal) {
        normalBuf.push_back(normal);
}
glm::vec4 FbxMeshInfo::getNormalAt(int index) const {
        return normalBuf.at(index);
}

int FbxMeshInfo::getNormalCount() const { return normalBuf.size(); }
// index
void FbxMeshInfo::addIndex(int index) { indexBuf.push_back(index); }

int FbxMeshInfo::getIndexAt(int index) const { return indexBuf.at(index); }

int FbxMeshInfo::getIndexCount() const { return indexBuf.size(); }

// texture
void FbxMeshInfo::addTexture(const std::shared_ptr<PngTexture>& texture) {
        textureBuf.push_back(texture);
}

std::shared_ptr<PngTexture> FbxMeshInfo::getTextureAt(int index) {
        return textureBuf.at(index);
}
int FbxMeshInfo::getTextureCount() { return textureBuf.size(); }
// material
void FbxMeshInfo::addMaterial(const FbxMaterial& material) {
        materialBuf.push_back(material);
}

FbxMaterial& FbxMeshInfo::getMaterialAt(int index) {
        return materialBuf[index];
}
int FbxMeshInfo::getMaterialCount() const { return materialBuf.size(); }
}  // namespace gel