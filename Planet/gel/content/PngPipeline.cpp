#include "PngPipeline.hpp"
#include "../device/AssetDatabase.hpp"
#include "../device/PngTexture.hpp"
#include "../device/TextureManager.hpp"
namespace gel {
PngPipeline::PngPipeline(const std::shared_ptr<TextureManager>& textureManager)
    : textureManager(textureManager) {}

bool PngPipeline::accept(const std::string& path) { return true; }

void PngPipeline::load(const std::string& path, Thread thread) {
        AssetDatabase::getOrNewAsset<PngTexture>(path)->load(path, thread);
}

void PngPipeline::unload(const std::string& path) {
        AssetDatabase::getOrNewAsset<PngTexture>(path)->unload();
}
}  // namespace gel