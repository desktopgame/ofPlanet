#include "WarpPoint.hpp"

const float WarpPoint::SCALE = 3.0f;

WarpPoint::WarpPoint(const std::shared_ptr<gel::Shader>& shader,
                     const gel::NameRule& nameRule)
    : circles(), shader(shader), nameRule(nameRule) {}

void WarpPoint::init(const glm::vec4 basePoint) {
        this->basePoint = basePoint;
        int len = 64 - basePoint.y;
        for (int i = 0; i < len; i += 3) {
                gel::Circle circle(shader, nameRule);
                glm::vec4 p(basePoint.x, basePoint.y + i, basePoint.z, 1);
                circle.init(p, glm::vec4(1.0f, 0.0f, 0.0f, 0.5f), SCALE);
                circles.push_back(circle);
        }
}

void WarpPoint::destroy() {
        auto iter = circles.begin();
        while (iter != circles.end()) {
                auto& v = *iter;
                v.destroy();
                ++iter;
        }
        circles.clear();
}

void WarpPoint::draw() {
        auto iter = circles.begin();
        while (iter != circles.end()) {
                auto& v = *iter;
                v.draw();
                ++iter;
        }
}

bool WarpPoint::isHit(const glm::vec3 position) const {
        float HALF = SCALE / 2.0f;
        return position.x >= basePoint.x - HALF &&
               position.x < (basePoint.x + HALF) &&
               position.z >= basePoint.z - HALF &&
               position.z < (basePoint.z + HALF);
}
