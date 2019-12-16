#include "CameraRegistry.hpp"

#include "Camera.hpp"
namespace planet {

std::unordered_map<std::string, std::shared_ptr<Camera> >
    CameraRegistry::cameraMap;
glm::vec2 CameraRegistry::defaultScreenSize;

std::shared_ptr<Camera> CameraRegistry::create(const std::string& name) {
        if (CameraRegistry::cameraMap.count(name)) {
                throw std::logic_error(name + "is already exists");
        }
        auto cam = std::make_shared<Camera>();
        cam->setScreenSize(CameraRegistry::defaultScreenSize);
        CameraRegistry::cameraMap.insert_or_assign(name, cam);
        return cam;
}

std::shared_ptr<Camera> CameraRegistry::get(const std::string& name) {
        return CameraRegistry::cameraMap.at(name);
}

std::shared_ptr<Camera> CameraRegistry::ref(const std::string& name) {
        return has(name) ? get(name) : create(name);
}

bool CameraRegistry::has(const std::string& name) {
        return CameraRegistry::cameraMap.count(name);
}

std::shared_ptr<Camera> CameraRegistry::main() { return ref("Main"); }

void CameraRegistry::setDefaultScreenSize(glm::vec2 defaultScreenSize,
                                          bool forceUpdate) {
        if (forceUpdate) {
                for (auto kv : CameraRegistry::cameraMap) {
                        kv.second->setScreenSize(defaultScreenSize);
                }
        }
        CameraRegistry::defaultScreenSize = defaultScreenSize;
}

glm::vec2 CameraRegistry::getDefaultScreenSize() {
        return CameraRegistry::defaultScreenSize;
}
}  // namespace planet
