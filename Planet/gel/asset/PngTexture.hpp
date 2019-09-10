#ifndef GEL_ASSET_PNGTEXTURE_HPP
#define GEL_ASSET_PNGTEXTURE_HPP
#include <glm/glm.hpp>
#include "ITexture.hpp"
namespace gel {

class PngTexture : public ITexture {
       public:
        PngTexture();
        ~PngTexture() = default;
        void load(const std::string& path, Thread thread) override;
        void unload() override;
        GLuint getID() const override;
        unsigned char* getData() const override;
        int getWidth() const override;
        int getHeight() const override;
        glm::vec2 getSize() const override;

       private:
        unsigned int width;
        unsigned int height;
        int depth;
        int colorType;
        int interlaceType;
        unsigned char* data;
        unsigned int textureId;
};
}  // namespace gel
#endif