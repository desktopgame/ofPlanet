#include "IRMesh.hpp"
#include "IRMaterial.hpp"
#include "IRShape.hpp"

namespace gel {
IRMesh::IRMesh()
    : children(), materials(), shape(std::make_shared<IRShape>()) {}
IRMesh::~IRMesh() {}
std::shared_ptr<IRMesh> IRMesh::addMesh(const std::shared_ptr<IRMesh>& mesh) {
        children.push_back(mesh);
        return mesh;
}

std::shared_ptr<IRMesh> IRMesh::addMesh() {
        return addMesh(std::make_shared<IRMesh>());
}
std::shared_ptr<IRMesh> IRMesh::getMesh(int index) const {
        return children.at(index);
}
void IRMesh::removeMesh(int index) { children.erase(children.begin() + index); }

size_t IRMesh::getMeshCount() const { return children.size(); }

std::shared_ptr<IRMaterial> IRMesh::addMaterial(
    const std::shared_ptr<IRMaterial>& material) {
        materials.push_back(material);
        return material;
}

std::shared_ptr<IRMaterial> IRMesh::addMaterial() {
        return addMaterial(std::make_shared<IRMaterial>());
}

std::shared_ptr<IRMaterial> IRMesh::getMaterial(int index) const {
        return materials.at(index);
}

void IRMesh::removeMaterial(int index) {
        materials.erase(materials.begin() + index);
}

size_t IRMesh::getMaterialCount() const { return materials.size(); }

std::shared_ptr<ITexture> IRMesh::addTexture(
    const std::shared_ptr<ITexture>& texture) {
        textures.push_back(texture);
        return texture;
}

std::shared_ptr<ITexture> IRMesh::getTexture(int index) const {
        return textures.at(index);
}

void IRMesh::removeTexture(int index) {
        textures.erase(textures.begin() + index);
}

size_t IRMesh::getTextureCount() const { return textures.size(); }

std::shared_ptr<IRShape> IRMesh::getShape() const { return shape; }

}  // namespace gel