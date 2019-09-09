#include "FbxPipeline.hpp"
#include "../Game.hpp"
#include "../device/AssetDatabase.hpp"
#include "../device/FbxModel.hpp"
namespace gel {
FbxPipeline::FbxPipeline() {}

bool FbxPipeline::accept(const std::string& path) { return true; }

void FbxPipeline::load(const std::string& path, Thread thread) {
        auto fbxMgr = Game::getInstance()->getFbxManager();
        auto tex = std::string("TextureFixed");
        auto color = std::string("ColorFixed");
        AssetDatabase::getOrNewAsset<FbxModel>(path, fbxMgr, tex, color,
                                               NameRule())
            ->load(path, thread);
}

void FbxPipeline::unload(const std::string& path) {
        AssetDatabase::removeAsset<FbxModel>(path)->unload();
}
}  // namespace gel