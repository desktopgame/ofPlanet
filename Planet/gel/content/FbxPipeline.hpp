#ifndef GEL_CONTENT_FBXPIPELINE_HPP
#define GEL_CONTENT_FBXPIPELINE_HPP
#include <memory>
#include <string>
#include "IContentPipeline.hpp"
namespace gel {
class FbxPipeline : public IContentPipeline {
       public:
        FbxPipeline();

        bool accept(const std::string& path) override;
        void load(const std::string& path, Thread thread) override;
        void unload(const std::string& path) override;

       private:
};
}  // namespace gel
#endif