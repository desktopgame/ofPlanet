#ifndef GEL_ASSET_ITEXTURE_HPP
#define GEL_ASSET_ITEXTURE_HPP
#include <glm/glm.hpp>
#include <string>
#include "../gli.hpp"
#include "IAsset.hpp"
namespace gel {
class ITexture : public IAsset {
       public:
        ITexture() = default;
        virtual ~ITexture() = default;
        virtual GLuint getID() const = 0;
        virtual unsigned char* getData() const = 0;
        virtual int getWidth() const = 0;
        virtual int getHeight() const = 0;
        virtual glm::vec2 getSize() const = 0;
};
}  // namespace gel
#endif