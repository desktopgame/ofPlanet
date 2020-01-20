#include "ShaderRegistry.hpp"

#include "../io/File.hpp"
namespace planet {
// ShaderRegistry
std::unordered_map<std::string, std::shared_ptr<Shader> > ShaderRegistry::map =
    std::unordered_map<std::string, std::shared_ptr<Shader> >();
void ShaderRegistry::loadFile(const std::string& name,
                              const std::string& vertFile,
                              const std::string& fragFile) {
        auto s = std::make_shared<Shader>();
        s->load(vertFile, fragFile);
        add(name, s);
}

void ShaderRegistry::loadString(const std::string& name,
                                const std::string& vertProgram,
                                const std::string& fragProgram) {
        throw std::logic_error("not supported");
}

void ShaderRegistry::add(const std::string& name,
                         const std::shared_ptr<Shader>& shader) {
        map.insert_or_assign(name, shader);
}

void ShaderRegistry::remove(const std::string& name) { map.erase(name); }
std::shared_ptr<Shader> ShaderRegistry::get(const std::string& name) {
        return map.at(name);
}
}  // namespace planet
