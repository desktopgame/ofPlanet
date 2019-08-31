#include "PlaneDraw.hpp"
#include <stdexcept>
namespace gel {
PlaneDraw::PlaneDraw() : map(), initFlag(false) {}

void PlaneDraw::init() {
        initFlag.check(false, "already initialized");
        map.insert_or_assign(PlaneType::Forward,
                             Buffer<float>(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW));
        map.insert_or_assign(PlaneType::Backward,
                             Buffer<float>(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW));
        map.insert_or_assign(PlaneType::Left,
                             Buffer<float>(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW));
        map.insert_or_assign(PlaneType::Right,
                             Buffer<float>(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW));
        map.insert_or_assign(PlaneType::Top,
                             Buffer<float>(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW));
        map.insert_or_assign(PlaneType::Bottom,
                             Buffer<float>(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW));
        auto iter = map.begin();
        while (iter != map.end()) {
                auto& val = *iter;
                val.second.init();
                ++iter;
        }
        initFlag.enable();
}

void PlaneDraw::destroy() {
        initFlag.check(true, "shoud be initalize");
        auto iter = map.begin();
        while (iter != map.end()) {
                auto& val = *iter;
                val.second.destroy();
                ++iter;
        }
        map.clear();
        initFlag.disable();
}

void PlaneDraw::clear() {
        initFlag.check(true, "shoud be initalize");
        auto iter = map.begin();
        while (iter != map.end()) {
                auto& val = *iter;
                val.second.modify().clear();
                ++iter;
        }
}

void PlaneDraw::add(PlaneType type, float x, float y, float z, float w) {
        initFlag.check(true, "shoud be initalize");
        map.at(type).modify().emitVec4(x, y, z, w);
}

void PlaneDraw::add(PlaneType type, glm::vec4 pos) {
        initFlag.check(true, "shoud be initalize");
        add(type, pos.x, pos.y, pos.z, pos.w);
}

void PlaneDraw::update() {
        initFlag.check(true, "shoud be initalize");
        auto iter = map.begin();
        while (iter != map.end()) {
                auto& val = *iter;
                val.second.bind();
                val.second.update();
                val.second.unbind();
                ++iter;
        }
}

void PlaneDraw::render(Plane& plane, const NameRule nameRule) {
        initFlag.check(true, "shoud be initalize");
        auto iter = map.begin();
        while (iter != map.end()) {
                auto& val = *iter;
                plane.setType(val.first);
                plane.render(val.second, val.second.size());
                ++iter;
        }
}
}  // namespace gel