#ifndef GEL_CONTENT_PNGPIPELINE_HPP
#define GEL_CONTENT_PNGPIPELINE_HPP
#include <memory>
#include <string>
#include "IContentPipeline.hpp"
namespace gel {
class PngPipeline : public IContentPipeline {
       public:
        PngPipeline();
        bool accept(const std::string& path) override;
        void load(const std::string& path, Thread thread) override;
        void unload(const std::string& path) override;

       private:
};
}  // namespace gel
#endif