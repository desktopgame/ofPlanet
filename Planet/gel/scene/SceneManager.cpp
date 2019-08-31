#include "SceneManager.hpp"
#include "IScene.hpp"

namespace gel {
SceneManager::SceneManager() : mSceneMap(), mCurrent() {}

void SceneManager::put(const std::string& name,
                       const std::shared_ptr<IScene>& scene) {
        this->mSceneMap[name] = scene;
}
void SceneManager::remove(const std::string& name) {
        this->mSceneMap.erase(name);
}
void SceneManager::bind(const std::string& name) {
        auto sceneOpt = getScene();
        if (!sceneOpt) {
                (*sceneOpt)->hide();
        }
        this->mCurrent = name;
        (*getScene())->show();
}

void SceneManager::update() {
        auto sceneOpt = getScene();
        if (!sceneOpt) {
                return;
        }
        auto v = *sceneOpt;
        v->update();
        if (v->isFinished()) {
                auto nname = v->getNextScene();
                auto n = mSceneMap[nname];
                v->hide();
                n->show();
                this->mCurrent = nname;
        }
}

void SceneManager::draw() {
        auto sceneOpt = getScene();
        if (sceneOpt) {
                auto v = *sceneOpt;
                v->draw();
        }
}

std::optional<std::shared_ptr<IScene> > SceneManager::getScene() {
        if (this->mSceneMap.count(mCurrent)) {
                return mSceneMap[mCurrent];
        }
        return nullptr;
}
}  // namespace gel