#ifndef GEL_ASSET_WAVEAUDIO_HPP
#define GEL_ASSET_WAVEAUDIO_HPP
#include <AL/alut.h>
#include "IAudio.hpp"
#include "WaveAudio.hpp"
namespace gel {
class WaveAudio : public IAudio {
       public:
        WaveAudio() = default;
        ~WaveAudio() = default;
        void load(const std::string& path, Thread thread) override;
        void play() const override;
        GLuint getBuffer() const override;
        GLuint getSource() const override;
        void unload() override;

       private:
        ALuint src;
        ALuint buf;
};
}  // namespace gel
#endif