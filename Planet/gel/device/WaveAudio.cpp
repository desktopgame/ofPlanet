#include "WaveAudio.hpp"
namespace gel {
void WaveAudio::loadOnAsync(const std::string& path) {}
void WaveAudio::loadOnGLThread(const std::string& path) {
        this->buf = alutCreateBufferFromFile(path.c_str());
        alGenSources(1, &(this->src));
        alSourcei(this->src, AL_BUFFER, this->buf);
}

void WaveAudio::play() const { alSourcePlay(this->src); }

GLuint WaveAudio::getBuffer() const { return this->buf; }

GLuint WaveAudio::getSource() const { return this->src; }

void WaveAudio::unload() {
        alDeleteBuffers(1, &(this->buf));
        alDeleteSources(1, &(this->src));
}
}  // namespace gel