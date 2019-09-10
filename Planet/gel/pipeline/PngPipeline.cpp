#include "PngPipeline.hpp"
#include "../asset/AssetDatabase.hpp"
#include "../asset/PngTexture.hpp"
namespace gel {
PngPipeline::PngPipeline() {}

bool PngPipeline::accept(const std::string& path) { return true; }

void PngPipeline::load(const std::string& path, Thread thread) {
        AssetDatabase::getOrNewAsset<PngTexture>(path)->load(path, thread);
}

void PngPipeline::unload(const std::string& path) {
        AssetDatabase::getOrNewAsset<PngTexture>(path)->unload();
}
}  // namespace gel