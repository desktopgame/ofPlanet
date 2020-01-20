#ifndef GEL_SHADER_SHADERRREGISTRY_HPP
#define GEL_SHADER_SHADERRREGISTRY_HPP
#include <memory>
#include <string>
#include <unordered_map>

#include "../shader/Shader.hpp"
namespace planet {

class Shader;
/**
 * ShaderRegistry は、シェーダを名前と紐付けて管理するレジストリです。
 */
class ShaderRegistry {
       public:
        static void loadFile(const std::string& name,
                             const std::string& vertFile,
                             const std::string& fragFile);
        static void loadString(const std::string& name,
                               const std::string& vertProgram,
                               const std::string& fragProgram);
        static void add(const std::string& name,
                        const std::shared_ptr<Shader>& shader);
        static void remove(const std::string& name);
        static std::shared_ptr<Shader> get(const std::string& name);

       private:
        ShaderRegistry() = delete;
        ~ShaderRegistry() = delete;
        static std::unordered_map<std::string, std::shared_ptr<Shader> > map;

       public:
        ShaderRegistry(const ShaderRegistry&) = delete;
        ShaderRegistry& operator=(const ShaderRegistry&) = delete;
        ShaderRegistry(ShaderRegistry&&) = delete;
        ShaderRegistry& operator=(ShaderRegistry&&) = delete;
};
}  // namespace planet
#endif