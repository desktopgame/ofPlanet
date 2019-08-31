#include "AABB.hpp"
#include <algorithm>
#include <limits>
#include "../gli.hpp"
namespace gel {
AABB::AABB() : minX(0), minY(0), minZ(0), maxX(1), maxY(1), maxZ(1) { init(); }
AABB::AABB(float minX, float minY, float minZ, float maxX, float maxY,
           float maxZ)
    : minX(minX), minY(minY), minZ(minZ), maxX(maxX), maxY(maxY), maxZ(maxZ) {
        init();
}

AABB::AABB(std::vector<glm::vec3> vertexes)
    : minX(std::numeric_limits<float>::max()),
      minY(std::numeric_limits<float>::max()),
      minZ(std::numeric_limits<float>::max()),
      maxX(std::numeric_limits<float>::min()),
      maxY(std::numeric_limits<float>::min()),
      maxZ(std::numeric_limits<float>::min()) {
        auto iter = vertexes.begin();
        while (iter != vertexes.end()) {
                auto v = (*iter);
                if (v.x < minX) {
                        minX = v.x;
                }
                if (v.y < minY) {
                        minY = v.y;
                }
                if (v.z < minZ) {
                        minZ = v.z;
                }
                if (v.x > maxX) {
                        maxX = v.x;
                }
                if (v.y > maxY) {
                        maxY = v.y;
                }
                if (v.z > maxZ) {
                        maxZ = v.z;
                }
                iter++;
        }
        init();
}

std::vector<glm::vec3> AABB::points() const {
        std::vector<glm::vec3> a;
        a.push_back(glm::vec3(minX, minY, maxZ));
        a.push_back(glm::vec3(maxX, minY, maxZ));

        a.push_back(glm::vec3(minX, maxY, maxZ));
        a.push_back(glm::vec3(maxX, maxY, maxZ));

        a.push_back(glm::vec3(minX, minY, minZ));
        a.push_back(glm::vec3(maxX, minY, minZ));

        a.push_back(glm::vec3(minX, maxY, minZ));
        a.push_back(glm::vec3(maxX, maxY, minZ));
        return a;
}

void AABB::drawQuad() const {
        glBegin(GL_QUADS);
        glVertex3f(this->maxX, this->maxY, this->minZ);
        glVertex3f(this->minX, this->maxY, this->minZ);
        glVertex3f(this->minX, this->maxY, this->maxZ);
        glVertex3f(this->maxX, this->maxY, this->maxZ);
        glVertex3f(this->maxX, this->minY, this->minZ);
        glVertex3f(this->minX, this->minY, this->minZ);
        glVertex3f(this->minX, this->minY, this->maxZ);
        glVertex3f(this->maxX, this->minY, this->maxZ);
        glVertex3f(this->maxX, this->maxY, this->maxZ);
        glVertex3f(this->minX, this->maxY, this->maxZ);
        glVertex3f(this->minX, this->minY, this->maxZ);
        glVertex3f(this->maxX, this->minY, this->maxZ);
        glVertex3f(this->maxX, this->maxY, this->minZ);
        glVertex3f(this->minX, this->maxY, this->minZ);
        glVertex3f(this->minX, this->minY, this->minZ);
        glVertex3f(this->maxX, this->minY, this->minZ);
        glVertex3f(this->minX, this->maxY, this->maxZ);
        glVertex3f(this->minX, this->maxY, this->minZ);
        glVertex3f(this->minX, this->minY, this->minZ);
        glVertex3f(this->minX, this->minY, this->maxZ);
        glVertex3f(this->maxX, this->maxY, this->maxZ);
        glVertex3f(this->maxX, this->maxY, this->minZ);
        glVertex3f(this->maxX, this->minY, this->minZ);
        glVertex3f(this->maxX, this->minY, this->maxZ);
        glEnd();
}

void AABB::drawFrame(int lineWidth) const {
        glLineWidth(lineWidth);
        glBegin(GL_LINES);
        glVertex3f(minX, minY, maxZ);
        glVertex3f(maxX, minY, maxZ);
        glEnd();

        glBegin(GL_LINES);
        glVertex3f(minX, maxY, maxZ);
        glVertex3f(maxX, maxY, maxZ);
        glEnd();

        glBegin(GL_LINES);
        glVertex3f(minX, minY, minZ);
        glVertex3f(maxX, minY, minZ);
        glEnd();

        glBegin(GL_LINES);
        glVertex3f(minX, maxY, minZ);
        glVertex3f(maxX, maxY, minZ);
        glEnd();

        glBegin(GL_LINES);
        glVertex3f(minX, minY, minZ);
        glVertex3f(minX, minY, maxZ);
        glEnd();

        glBegin(GL_LINES);
        glVertex3f(minX, maxY, minZ);
        glVertex3f(minX, maxY, maxZ);
        glEnd();

        glBegin(GL_LINES);
        glVertex3f(maxX, minY, minZ);
        glVertex3f(maxX, minY, maxZ);
        glEnd();

        glBegin(GL_LINES);
        glVertex3f(maxX, maxY, minZ);
        glVertex3f(maxX, maxY, maxZ);
        glEnd();

        glBegin(GL_LINES);
        glVertex3f(minX, minY, minZ);
        glVertex3f(minX, maxY, minZ);
        glEnd();

        glBegin(GL_LINES);
        glVertex3f(maxX, minY, minZ);
        glVertex3f(maxX, maxY, minZ);
        glEnd();

        glBegin(GL_LINES);
        glVertex3f(minX, minY, maxZ);
        glVertex3f(minX, maxY, maxZ);
        glEnd();

        glBegin(GL_LINES);
        glVertex3f(maxX, minY, maxZ);
        glVertex3f(maxX, maxY, maxZ);
        glEnd();
}

glm::vec3 AABB::getSize() const {
        return glm::vec3(getSizeX(), getSizeY(), getSizeZ());
}

float AABB::getSizeX() const { return maxX - minX; }

float AABB::getSizeY() const { return maxY - minY; }

float AABB::getSizeZ() const { return maxZ - minZ; }

AABB AABB::scaled(const glm::vec3& scale) const {
        AABB ret;
        ret.minX = minX * scale.x;
        ret.maxX = maxX * scale.x;
        ret.minY = minY * scale.y;
        ret.maxY = maxY * scale.y;
        ret.minZ = minZ * scale.z;
        ret.maxZ = maxZ * scale.z;
        return ret;
}

AABB AABB::transform(const glm::mat4& m) const {
        auto v = points();
        auto iter = v.begin();
        std::vector<glm::vec3> d;
        while (iter != v.end()) {
                glm::vec3 e = (*iter);
                glm::vec4 tmp = glm::vec4(e.x, e.y, e.z, 1);
                d.push_back(tmp * m);
                ++iter;
        }
        return AABB(d);
}

bool AABB::isIntersects(const AABB& a, const AABB& b) {
        return (a.minX <= b.maxX && a.maxX >= b.minX) &&
               (a.minY <= b.maxY && a.maxY >= b.minY) &&
               (a.minZ <= b.maxZ && a.maxZ >= b.minZ);
}

glm::vec3 AABB::clamp(const glm::vec3& pos) const {
        return glm::vec3(std::min(maxX, std::max(minX, pos.x)),
                         std::min(maxY, std::max(minY, pos.y)),
                         std::min(maxZ, std::max(minZ, pos.z)));
}

void AABB::init() {
        this->center =
            glm::vec3(minX + (maxX - minX) / 2, minY + (maxY - minY) / 2,
                      minZ + (maxZ - minZ) / 2);
        this->radius =
            glm::vec3((maxX - minX) / 2, (maxY - minY) / 2, (maxZ - minZ) / 2);
}
}  // namespace gel