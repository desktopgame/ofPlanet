#ifndef GEL_PIPELINE_ICONTENTPIPELINE_HPP
#define GEL_PIPELINE_ICONTENTPIPELINE_HPP
#include <string>
#include "../Thread.hpp"
namespace gel {
/**
 * IContentPipeline is core of asset load system.
 * in general, using with ContentManager.
 */
class IContentPipeline {
       public:
        IContentPipeline() = default;
        virtual ~IContentPipeline() = default;
        virtual bool accept(const std::string& path) = 0;
        virtual void load(const std::string& path, Thread thread) = 0;
        virtual void unload(const std::string& path) = 0;
};
}  // namespace gel
#endif