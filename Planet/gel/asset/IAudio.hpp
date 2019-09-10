#ifndef GEL_ASSET_AUDIO_HPP
#define GEL_ASSET_AUDIO_HPP
#include <string>
#include "../gli.hpp"
#include "IAsset.hpp"
namespace gel {
class IAudio : public IAsset {
       public:
        IAudio() = default;
        virtual ~IAudio() = default;
        virtual void play() const = 0;
        virtual GLuint getBuffer() const = 0;
        virtual GLuint getSource() const = 0;
};
}  // namespace gel
#endif