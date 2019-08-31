#include "EchoPipeline.hpp"
#include <iostream>
namespace gel {
bool EchoPipeline::accept(const std::string& path) { return true; }
void EchoPipeline::load(const std::string& path) {
        std::cout << path << std::endl;
}
void EchoPipeline::unload(const std::string& path) {}
}  // namespace gel