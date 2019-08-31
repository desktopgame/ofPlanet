#include "PngPipeline.hpp"
#include "../device/TextureManager.hpp"
namespace gel {
PngPipeline::PngPipeline(const std::shared_ptr<TextureManager>& textureManager)
    : textureManager(textureManager) {}

bool PngPipeline::accept(const std::string& path) { return true; }

void PngPipeline::load(const std::string& path) {
        textureManager->loadPng(path);
}

void PngPipeline::unload(const std::string& path) {
        textureManager->unload(path);
}
}  // namespace gel