#ifndef GEL_PIPELINE_BMPPIPELINE_HPP
#define GEL_PIPELINE_BMPPIPELINE_HPP
#include <memory>
#include <string>
#include "IContentPipeline.hpp"
namespace gel {
class BmpPipeline : public IContentPipeline {
       public:
        BmpPipeline();
        bool accept(const std::string& path) override;
        void load(const std::string& path, Thread thread) override;
        void unload(const std::string& path) override;

       private:
};
}  // namespace gel
#endif