#ifndef GEL_CONTENT_ECHOPIPELINE_HPP
#define GEL_CONTENT_ECHOPIPELINE_HPP
#include "IContentPipeline.hpp"
namespace gel {

class EchoPipeline : public IContentPipeline {
       public:
        EchoPipeline() = default;
        bool accept(const std::string& path) override;
        void load(const std::string& path) override;
        void unload(const std::string& path) override;
};
}  // namespace gel
#endif