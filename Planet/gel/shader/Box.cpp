#include "Box.hpp"
namespace gel {
Box::Box(const std::shared_ptr<Shader>& shader, const NameRule& nameRule)
    : plane(shader, nameRule) {}

void Box::init(const glm::vec4 color, glm::vec3 scale) { plane.init(scale); }

void Box::destroy() { plane.destroy(); }

void Box::draw() {
        plane.setType(PlaneType::Forward);
        plane.draw();

        plane.setType(PlaneType::Backward);
        plane.draw();

        plane.setType(PlaneType::Left);
        plane.draw();

        plane.setType(PlaneType::Right);
        plane.draw();

        plane.setType(PlaneType::Top);
        plane.draw();

        plane.setType(PlaneType::Bottom);
        plane.draw();
}
}  // namespace gel