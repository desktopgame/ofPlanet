#include "ModelManager.hpp"
#include <stdio.h>
#include <stdexcept>
#include "../Game.hpp"
#include "FbxModel.hpp"
namespace gel {
ModelManager::ModelManager() : modelMap() {}

ModelManager::~ModelManager() {}

void ModelManager::loadFbx(const std::string& path) {
        auto fbxModel = std::make_shared<FbxModel>(
            Game::getInstance()->getFbxManager(), "TextureFixed", "ColorFixed",
            NameRule());
        fbxModel->load(path, Thread::OnBackground);
        fbxModel->load(path, Thread::OnGL);
        modelMap[path] = fbxModel;
}

void ModelManager::unload(const std::string& path) {
        if (modelMap.count(path)) {
                auto fbxModel = modelMap.at(path);
                modelMap.erase(path);
                fbxModel->unload();
        }
}

std::shared_ptr<IModel> ModelManager::getModel(const std::string& path) const {
        if (modelMap.count(path)) {
                return modelMap.at(path);
        }
        return nullptr;
}
}  // namespace gel