#ifndef GEL_ASSET_BMPTEXTURE_HPP
#define GEL_ASSET_BMPTEXTURE_HPP
#include "ITexture.hpp"
namespace gel {
class BmpTexture : public ITexture {
       public:
        BmpTexture();
        ~BmpTexture() = default;
        void load(const std::string& path, Thread thread) override;
        void unload() override;
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