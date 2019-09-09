#include "BmpPipeline.hpp"
#include "../device/AssetDatabase.hpp"
#include "../device/BmpTexture.hpp"
namespace gel {
BmpPipeline::BmpPipeline() {}

bool BmpPipeline::accept(const std::string& path) { return true; }

void BmpPipeline::load(const std::string& path, Thread thread) {
        AssetDatabase::getOrNewAsset<BmpTexture>(path)->load(path, thread);
}

void BmpPipeline::unload(const std::string& path) {
        AssetDatabase::removeAsset<BmpTexture>(path)->unload();
}
}  // namespace gel