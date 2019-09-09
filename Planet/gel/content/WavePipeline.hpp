#ifndef GEL_CONTENT_WAVEPIPELINE_HPP
#define GEL_CONTENT_WAVEPIPELINE_HPP
#include <memory>
#include "IContentPipeline.hpp"

namespace gel {
class WavePipeline : public IContentPipeline {
       public:
        WavePipeline();
        bool accept(const std::string& path) override;
        void load(const std::string& path, Thread thread) override;
        void unload(const std::string& path) override;

       private:
};
}  // namespace gel
#endif