#pragma once
#ifndef GEL_DEVICE_JPEGTEXTURE_HPP
#define GEL_DEVICE_JPEGTEXTURE_HPP
#include "ITexture.hpp"
namespace gel {
class JpegTexture : public ITexture {
       public:
        JpegTexture();
        ~JpegTexture() = default;
        void loadOnAsync(const std::string& path) override;
        void loadOnGLThread(const std::string& path) override;
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