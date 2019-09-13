#ifndef GEL_SHADER_SHADERRREGISTRY_HPP
#define GEL_SHADER_SHADERRREGISTRY_HPP
#include <memory>
#include <string>
#include <unordered_map>
#include "../shader/Shader.hpp"

namespace gel {
class ShaderRegistry {
       public:
        void put(const std::string& name, const std::string& vertFile,
                 const std::string& fragFile);
        void put(const std::string& name,
                 const std::shared_ptr<Shader>& shader);
        void remove(const std::string& name);
        std::shared_ptr<Shader> get(const std::string& name);

       private:
        ShaderRegistry();
        ~ShaderRegistry();
        std::unordered_map<std::string, std::shared_ptr<Shader> > map;

       public:
        ShaderRegistry(const ShaderRegistry&) = delete;
        ShaderRegistry& operator=(const ShaderRegistry&) = delete;
        ShaderRegistry(ShaderRegistry&&) = delete;
        ShaderRegistry& operator=(ShaderRegistry&&) = delete;

        static ShaderRegistry& getInstance();
};
}  // namespace gel
#endif