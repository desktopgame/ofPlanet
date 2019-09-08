#ifndef GEL_DEVICE_AUDIO_HPP
#define GEL_DEVICE_AUDIO_HPP
#include <string>
#include "../gli.hpp"
namespace gel {
class IAudio {
       public:
        IAudio() = default;
        virtual ~IAudio() = default;
        virtual void load(const std::string& path) = 0;
        virtual void play() const = 0;
        virtual GLuint getBuffer() const = 0;
        virtual GLuint getSource() const = 0;
        virtual void unload() = 0;
};
}  // namespace gel
#endif