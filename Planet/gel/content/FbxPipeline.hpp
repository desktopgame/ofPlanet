#ifndef GEL_CONTENT_FBXPIPELINE_HPP
#define GEL_CONTENT_FBXPIPELINE_HPP
#include <memory>
#include <string>
#include "IContentPipeline.hpp"
namespace gel {
class ModelManager;
class FbxPipeline : public IContentPipeline {
       public:
        FbxPipeline(const std::shared_ptr<ModelManager>& modelManager);

        bool accept(const std::string& path) override;
        void load(const std::string& path, Thread thread) override;
        void unload(const std::string& path) override;

       private:
        std::shared_ptr<ModelManager> modelManager;
};
}  // namespace gel
#endif