#ifndef GEL_SHADER_SHADERRREGISTRY_HPP
#define GEL_SHADER_SHADERRREGISTRY_HPP
#include <memory>
#include <string>
#include <unordered_map>
#include "../shader/Shader.hpp"

namespace gel {
class ShaderRef {
public:
	ShaderRef(const std::shared_ptr<Shader> shader);
	~ShaderRef();
	std::shared_ptr<Shader> shader;
};
class ShaderRegistry {
       public:
        static void put(const std::string& name, const std::string& vertFile,
                 const std::string& fragFile);
        static void put(const std::string& name,
                 const std::shared_ptr<Shader>& shader);
        static void remove(const std::string& name);
        static std::shared_ptr<Shader> get(const std::string& name);

       private:
        ShaderRegistry() = delete;
        ~ShaderRegistry() = delete;
        static std::unordered_map<std::string, ShaderRef > map;

       public:
        ShaderRegistry(const ShaderRegistry&) = delete;
        ShaderRegistry& operator=(const ShaderRegistry&) = delete;
        ShaderRegistry(ShaderRegistry&&) = delete;
        ShaderRegistry& operator=(ShaderRegistry&&) = delete;
};
}  // namespace gel
#endif