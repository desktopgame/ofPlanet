#include "BmpPipeline.hpp"
#include "../device/TextureManager.hpp"
namespace gel {
BmpPipeline::BmpPipeline(const std::shared_ptr<TextureManager>& textureManager)
    : textureManager(textureManager) {}

bool BmpPipeline::accept(const std::string& path) { return true; }

void BmpPipeline::load(const std::string& path, Thread thread) {
        textureManager->loadBmp(path);
}

void BmpPipeline::unload(const std::string& path) {
        textureManager->unload(path);
}
}  // namespace gel