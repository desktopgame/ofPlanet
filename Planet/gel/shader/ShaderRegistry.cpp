#include "ShaderRegistry.hpp"
#include "../util/io.hpp"
namespace gel {
ShaderRegistry::ShaderRegistry() : map() {}

ShaderRegistry::~ShaderRegistry() {
        auto iter = map.begin();
        while (iter != map.end()) {
                auto& p = *iter;
                p.second->unlink();
                ++iter;
        }
}

void ShaderRegistry::put(const std::string& name, const std::string& vertFile,
                         const std::string& fragFile) {
        auto s = std::make_shared<Shader>();
        s->link(gel::readAllText(vertFile), gel::readAllText(fragFile));
        put(name, s);
}

void ShaderRegistry::put(const std::string& name,
                         const std::shared_ptr<Shader>& shader) {
        map.insert_or_assign(name, shader);
}

void ShaderRegistry::remove(const std::string& name) { map.erase(name); }
std::shared_ptr<Shader> ShaderRegistry::get(const std::string& name) {
        return map.at(name);
}

ShaderRegistry& ShaderRegistry::getInstance() {
        static ShaderRegistry instance;
        return instance;
}
}  // namespace gel