#ifndef GEL_CONTENT_JPEGPIPELINE_HPP
#define GEL_CONTENT_JPEGPIPELINE_HPP
#include "IContentPipeline.hpp"

namespace gel {
class JpegPipeline : public IContentPipeline {
       public:
        JpegPipeline();
        bool accept(const std::string& path) override;
        void load(const std::string& path, Thread thread) override;
        void unload(const std::string& path) override;

       private:
};
}  // namespace gel
#endif