#ifndef GEL_CONTENT_JPEGPIPELINE_HPP
#define GEL_CONTENT_JPEGPIPELINE_HPP
#include "IContentPipeline.hpp"

namespace gel {
class TextureManager;
class JpegPipeline : public IContentPipeline {
       public:
        JpegPipeline(const std::shared_ptr<TextureManager>& textureManager);
        bool accept(const std::string& path) override;
        void load(const std::string& path) override;
        void unload(const std::string& path) override;

       private:
        std::shared_ptr<TextureManager> textureManager;
};
}  // namespace gel
#endif