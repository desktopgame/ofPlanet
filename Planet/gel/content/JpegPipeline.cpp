#include "JpegPipeline.hpp"
#include "../device/TextureManager.hpp"
namespace gel {
JpegPipeline::JpegPipeline(
    const std::shared_ptr<TextureManager>& textureManager)
    : textureManager(textureManager) {}

bool JpegPipeline::accept(const std::string& path) { return true; }
void JpegPipeline::load(const std::string& path, Thread thread) {
        textureManager->loadJpeg(path);
}
void JpegPipeline::unload(const std::string& path) {
        textureManager->unload(path);
}
}  // namespace gel