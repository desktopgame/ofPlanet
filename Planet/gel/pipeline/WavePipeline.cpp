#include "WavePipeline.hpp"
#include "../asset/AssetDatabase.hpp"
#include "../asset/WaveAudio.hpp"
namespace gel {
WavePipeline::WavePipeline() {}
bool WavePipeline::accept(const std::string& path) { return true; }

void WavePipeline::load(const std::string& path, Thread thread) {
        AssetDatabase::getOrNewAsset<WaveAudio>(path)->load(path, thread);
}

void WavePipeline::unload(const std::string& path) {
        AssetDatabase::getOrNewAsset<WaveAudio>(path)->unload();
}
}  // namespace gel
