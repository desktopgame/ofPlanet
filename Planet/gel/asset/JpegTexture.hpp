#pragma once
#ifndef GEL_ASSET_JPEGTEXTURE_HPP
#define GEL_ASSET_JPEGTEXTURE_HPP
#include "ITexture.hpp"
namespace gel {
class JpegTexture : public ITexture {
       public:
        JpegTexture();
        ~JpegTexture() = default;
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
        int ch;
        unsigned char* data;
        unsigned int textureId;
};
}  // namespace gel
#endif