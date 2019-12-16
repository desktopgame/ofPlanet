#pragma once
#ifndef SHADER_MATERIALREGISTRY_HPP
#define SHADER_MATERIALREGISTRY_HPP
#include <memory>
#include <string>
#include <unordered_map>
namespace planet {

class Material;
class MaterialRegistry {
       public:
        static std::shared_ptr<Material> create(const std::string& name);
        static std::shared_ptr<Material> get(const std::string& name);
        static std::shared_ptr<Material> ref(const std::string& name);
        static bool has(const std::string& name);

       private:
        static std::unordered_map<std::string, std::shared_ptr<Material> >
            matMap;
        MaterialRegistry() = delete;
        ~MaterialRegistry() = delete;
};
}  // namespace planet
#endif