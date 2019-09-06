#include "IRModel.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "IRMesh.hpp"
namespace gel {
std::shared_ptr<IRModel> IRModel::create() {
        auto ret = SharedHelper<IRModel>::make_shared();
        ret->mesh = std::make_shared<IRMesh>(ret, "RootNode");
        return ret;
}
IRModel::~IRModel() {}

std::shared_ptr<IRMesh> IRModel::getMesh() const { return mesh; }
void IRModel::setModelMatrix(const glm::mat4 modelMatrix) {
        this->modelMatrix = modelMatrix;
}
glm::mat4 IRModel::getModelMatrix() const { return modelMatrix; }
void IRModel::setViewMatrix(const glm::mat4 viewMatrix) {
        this->viewMatrix = viewMatrix;
}
glm::mat4 IRModel::getViewMatrix() const { return viewMatrix; }
void IRModel::setProjectionMatrix(const glm::mat4 projectionMatrix) {
        this->projectionMatrix = projectionMatrix;
}
glm::mat4 IRModel::getProjectionMatrix() const { return projectionMatrix; }
glm::mat4 IRModel::getNormalMatrix() const {
        glm::mat4 normalMatrix = viewMatrix * modelMatrix;
        normalMatrix = glm::inverse(normalMatrix);
        normalMatrix = glm::transpose(normalMatrix);
        return normalMatrix;
}
// private
IRModel::IRModel()
    : mesh(nullptr),
      modelMatrix(glm::mat4(1.0f)),
      viewMatrix(glm::mat4(1.0f)),
      projectionMatrix(glm::mat4(1.0f)) {}

}  // namespace gel