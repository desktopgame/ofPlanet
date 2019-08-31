#ifndef GEL_DEVICE_BMPTEXTURE_HPP
#define GEL_DEVICE_BMPTEXTURE_HPP
#include "ITexture.hpp"
namespace gel {
class BmpTexture : public ITexture {
       public:
        BmpTexture();
        void load(const std::string& path) override;
        void unload(const std::string& path) override;
        GLuint getID() const override;
        unsigned char* getData() const override;
        int getWidth() const override;
        int getHeight() const override;
        glm::vec2 getSize() const override;

       private:
        void readBMP(const char* filename);
        GLuint tid;
        int width;
        int height;
        unsigned char* data;
};
}  // namespace  gel
#endif