#include "AudioManager.hpp"
#include "WaveAudio.hpp"

namespace gel {
AudioManager::AudioManager() : audioMap() {}
void AudioManager::loadWave(const std::string& path) {
        auto playable = std::make_shared<WaveAudio>();
        playable->load(path);
        audioMap[path] = playable;
}

void AudioManager::play(const std::string& path) const {
        if (audioMap.count(path)) {
                audioMap.at(path)->play();
        }
}

GLuint AudioManager::getBuffer(const std::string& path) const {
        if (audioMap.count(path)) {
                return audioMap.at(path)->getBuffer();
        }
        ::abort();
}

GLuint AudioManager::getSource(const std::string& path) const {
        if (audioMap.count(path)) {
                return audioMap.at(path)->getSource();
        }
        ::abort();
}

void AudioManager::unload(const std::string& path) {
        if (audioMap.count(path)) {
                audioMap.at(path)->unload(path);
                audioMap.erase(path);
        }
}
}  // namespace gel