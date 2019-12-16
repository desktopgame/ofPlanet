#pragma once
#ifndef SHADER_CAMERAREGISTRY_HPP
#define SHADER_CAMERAREGISTRY_HPP
#include <ofVec2f.h>

#include <memory>
#include <string>
#include <unordered_map>
namespace planet {

class Camera;
class CameraRegistry {
       public:
        static std::shared_ptr<Camera> create(const std::string& name);
        static std::shared_ptr<Camera> get(const std::string& name);
        static std::shared_ptr<Camera> ref(const std::string& name);
        static bool has(const std::string& name);
        static std::shared_ptr<Camera> main();

        static void setDefaultScreenSize(glm::vec2 defaultScreenSize,
                                         bool forceUpdate);
        static glm::vec2 getDefaultScreenSize();

       private:
        static std::unordered_map<std::string, std::shared_ptr<Camera> >
            cameraMap;
        static glm::vec2 defaultScreenSize;
        CameraRegistry() = delete;
        ~CameraRegistry() = delete;
};
}  // namespace planet
#endif