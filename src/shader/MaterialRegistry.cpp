#include "MaterialRegistry.hpp"

#include "Material.hpp"
namespace planet {

std::unordered_map<std::string, std::shared_ptr<Material> >
    MaterialRegistry::matMap;

std::shared_ptr<Material> MaterialRegistry::create(const std::string& name) {
        if (MaterialRegistry::matMap.count(name)) {
                throw std::logic_error(name + "is already exists");
        }
        auto cam = std::make_shared<Material>();
        MaterialRegistry::matMap.insert_or_assign(name, cam);
        return cam;
}

std::shared_ptr<Material> MaterialRegistry::get(const std::string& name) {
        return MaterialRegistry::matMap.at(name);
}

std::shared_ptr<Material> MaterialRegistry::ref(const std::string& name) {
        return has(name) ? get(name) : create(name);
}

bool MaterialRegistry::has(const std::string& name) {
        return MaterialRegistry::matMap.count(name);
}
}  // namespace planet
