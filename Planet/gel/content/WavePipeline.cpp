#include "WavePipeline.hpp"
#include "../device/AssetDatabase.hpp"
#include "../device/WaveAudio.hpp"
namespace gel {
WavePipeline::WavePipeline(const std::shared_ptr<AudioManager>& audioManager)
    : audioManager(audioManager) {}
bool WavePipeline::accept(const std::string& path) { return true; }

void WavePipeline::load(const std::string& path, Thread thread) {
        AssetDatabase::getOrNewAsset<WaveAudio>(path)->load(path, thread);
}

void WavePipeline::unload(const std::string& path) {
        AssetDatabase::getOrNewAsset<WaveAudio>(path)->unload();
}
}  // namespace gel
