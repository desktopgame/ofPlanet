#ifndef GEL_CONTENT_ICONTENTPIPELINE_HPP
#define GEL_CONTENT_ICONTENTPIPELINE_HPP
#include <string>
namespace gel {
class IContentPipeline {
       public:
        IContentPipeline() = default;
        virtual ~IContentPipeline() = default;
        virtual bool accept(const std::string& path) = 0;
        virtual void load(const std::string& path) = 0;
        virtual void unload(const std::string& path) = 0;
};
}  // namespace gel
#endif