#include "LineRenderer.hpp"
#include "../gli.hpp"
namespace gel {
LineRenderer::LineRenderer() : positions() {}

void LineRenderer::addPos(const glm::vec3& pos) { positions.push_back(pos); }

void LineRenderer::setPos(int index, const glm::vec3& pos) {
        positions.assign(index, pos);
}

void LineRenderer::clear() { positions.clear(); }

size_t LineRenderer::size() const { return positions.size(); }

void LineRenderer::draw(int lineWidth) {
        glLineWidth(lineWidth);
        glPushMatrix();
        glBegin(GL_LINE_STRIP);
        for (auto vpos : positions) {
                glVertex3f(vpos.x, vpos.y, vpos.z);
        }
        glEnd();
        glPopMatrix();
}
}  // namespace gel