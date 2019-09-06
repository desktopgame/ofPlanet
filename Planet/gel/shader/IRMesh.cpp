#include "IRMesh.hpp"
#include "IRMaterial.hpp"
#include "IRShape.hpp"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/raw_data.hpp>

namespace gel {
IRMesh::IRMesh(const std::weak_ptr<IRModel>& model, const std::string& name)
    : model(model),
      children(),
      materials(),
      shape(std::make_shared<IRShape>()),
      name(name),
      position(),
      rotation(),
      scale() {}
IRMesh::~IRMesh() {}
std::shared_ptr<IRMesh> IRMesh::addMesh(const std::shared_ptr<IRMesh>& mesh) {
        children.push_back(mesh);
        mesh->parent = shared_from_this();
        return mesh;
}

std::shared_ptr<IRMesh> IRMesh::addMesh(const std::string& name) {
        return addMesh(std::make_shared<IRMesh>(this->model, name));
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

std::string IRMesh::getName() const { return name; }

std::shared_ptr<IRShape> IRMesh::getShape() const { return shape; }

void IRMesh::setLocalPosition(const glm::vec4 position) {
        this->position = position;
}

glm::vec4 IRMesh::getLocalPosition() const { return position; }

void IRMesh::setLocalRotation(const glm::vec4 rotation) {
        this->rotation = rotation;
}

glm::vec4 IRMesh::getLocalRotation() const { return rotation; }

void IRMesh::setLocalScale(const glm::vec4 scale) { this->scale = scale; }

glm::vec4 IRMesh::getLocalScale() const { return scale; }

glm::mat4 IRMesh::getLocalMatrix() const {
        glm::mat4 mat = glm::mat4(1.0f);
        mat =
            glm::translate(mat, glm::vec3(position.x, position.y, position.z));
        mat = glm::rotate(mat, rotation.y, glm::vec3(0, 1, 0));
        mat = glm::rotate(mat, rotation.x, glm::vec3(1, 0, 0));
        mat = glm::rotate(mat, rotation.z, glm::vec3(0, 0, 1));
        mat = glm::scale(mat, glm::vec3(scale.x, scale.y, scale.z));
        return mat;
}

glm::mat4 IRMesh::getTreeMatrix() const {
        auto p = parent.lock();
        if (p) {
                return p->getTreeMatrix() * getLocalMatrix();
        }
        return getLocalMatrix();
}

std::weak_ptr<IRMesh> IRMesh::getParent() const { return parent; }

std::weak_ptr<IRModel> IRMesh::getModel() const { return model; }

}  // namespace gel