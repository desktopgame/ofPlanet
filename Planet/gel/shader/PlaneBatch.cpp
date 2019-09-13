#include "PlaneBatch.hpp"
#include <stdexcept>
namespace gel {
PlaneBatch::PlaneBatch(const std::shared_ptr<Shader>& shader,
                       const NameRule& nameRule)
    : nameRule(nameRule),
      map(),
      shader(shader),
      plane(shader, nameRule),
      initFlag(false) {}

PlaneBatch::PlaneBatch(const std::shared_ptr<Shader>& shader)
    : PlaneBatch(shader, NameRule()) {}

void PlaneBatch::init(float size) {
        initFlag.check(false, "already initialized");
        plane.init(size);
        initFlag.enable();
}

void PlaneBatch::destroy() {
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

void PlaneBatch::clear() {
        initFlag.check(true, "shoud be initalize");
        auto iter = map.begin();
        while (iter != map.end()) {
                auto& val = *iter;
                val.second.clear();
                ++iter;
        }
}

PlaneDraw& PlaneBatch::get(GLuint id) {
        initFlag.check(true, "shoud be initalize");
        if (map.count(id) == 0) {
                PlaneDraw draw;
                draw.init();
                map.insert_or_assign(id, draw);
        }
        return map.at(id);
}

void PlaneBatch::render() {
        initFlag.check(true, "shoud be initalize");
        auto iter = map.begin();
        while (iter != map.end()) {
                auto& val = *iter;
                glBindTexture(GL_TEXTURE_2D, val.first);
                val.second.update();
                val.second.render(plane, nameRule);
                ++iter;
        }
}
}  // namespace gel