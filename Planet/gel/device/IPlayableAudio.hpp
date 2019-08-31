#ifndef GEL_DEVICE_PLAYABLEAUDIO_HPP
#define GEL_DEVICE_PLAYABLEAUDIO_HPP
#include <string>
#include "../gli.hpp"
namespace gel {
class IPlayableAudio {
       public:
        IPlayableAudio() = default;
        virtual ~IPlayableAudio() = default;
        virtual void load(const std::string& path) = 0;
        virtual void play() const = 0;
        virtual GLuint getBuffer() const = 0;
        virtual GLuint getSource() const = 0;
        virtual void unload(const std::string& path) = 0;
};
}  // namespace gel
#endif