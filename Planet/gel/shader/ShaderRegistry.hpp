#ifndef GEL_SHADER_SHADERRREGISTRY_HPP
#define GEL_SHADER_SHADERRREGISTRY_HPP
#include <string>
#include <unordered_map>
#include "../shader/Shader.hpp"

namespace gel {
class ShaderRegistry {
       public:
        void put(const std::string& name, const std::string& vertFile,
                 const std::string& fragFile);
        void put(const std::string& name, Shader& shader);
        void remove(const std::string& name);
        Shader& get(const std::string& name);

       private:
        ShaderRegistry();
        ~ShaderRegistry();
        std::unordered_map<std::string, Shader> map;

       public:
        ShaderRegistry(const ShaderRegistry&) = delete;
        ShaderRegistry& operator=(const ShaderRegistry&) = delete;
        ShaderRegistry(ShaderRegistry&&) = delete;
        ShaderRegistry& operator=(ShaderRegistry&&) = delete;

        static ShaderRegistry& getInstance();
};
}  // namespace gel
#endif