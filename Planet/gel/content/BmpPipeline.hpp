#ifndef GEL_CONTENT_BMPPIPELINE_HPP
#define GEL_CONTENT_BMPPIPELINE_HPP
#include <memory>
#include <string>
#include "IContentPipeline.hpp"
namespace gel {
class TextureManager;
class BmpPipeline : public IContentPipeline {
       public:
        BmpPipeline(const std::shared_ptr<TextureManager>& textureManager);
        bool accept(const std::string& path) override;
        void load(const std::string& path, Thread thread) override;
        void unload(const std::string& path) override;

       private:
        std::shared_ptr<TextureManager> textureManager;
};
}  // namespace gel
#endif