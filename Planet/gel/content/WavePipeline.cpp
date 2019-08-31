#include "WavePipeline.hpp"
namespace gel {
WavePipeline::WavePipeline(const std::shared_ptr<AudioManager>& audioManager)
    : audioManager(audioManager) {}
bool WavePipeline::accept(const std::string& path) { return true; }

void WavePipeline::load(const std::string& path) {
        audioManager->loadWave(path);
}

void WavePipeline::unload(const std::string& path) {
        audioManager->unload(path);
}
}  // namespace gel
