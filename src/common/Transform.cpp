#include "Transform.hpp"
namespace planet {

Transform::Transform() : position(), scale(1, 1, 1), rotation() {}

glm::vec3 Transform::forward() const {
        const float DEG2RAD = 3.14f / 180.0f;
        auto ax =
            std::sin(rotation.x * DEG2RAD) * std::cos(rotation.y * DEG2RAD) * 1;
        auto ay = std::sin(rotation.y * DEG2RAD);
        auto az =
            std::cos(rotation.x * DEG2RAD) * std::cos(rotation.y * DEG2RAD) * 1;
        return glm::vec3(ax, ay, az);
}

glm::vec3 Transform::backward() const { return -forward(); }

glm::vec3 Transform::left() const { return -right(); }

glm::vec3 Transform::right() const {
        auto fwd = forward();
        return glm::vec3(-fwd.z, 0, fwd.x);
}
}  // namespace planet
