#include "IRModel.hpp"
#include "IRMesh.hpp"
namespace gel {
IRModel::IRModel() : mesh(std::make_shared<IRMesh>()) {}

IRModel::~IRModel() {}

std::shared_ptr<IRMesh> IRModel::getMesh() const { return mesh; }
}  // namespace gel