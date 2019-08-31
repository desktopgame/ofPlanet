#include "EntityPhysics.hpp"
#include "World.hpp"
EntityPhysics::EntityPhysics() : _isGround(false) {}
void EntityPhysics::update(World& world) { applyPhysics(world); }
// protected
glm::vec3 EntityPhysics::getHitBlock(World& world, glm::vec3 npos,
                                     bool* outHit) {
        auto pos = transform.position;
        int x = Block::floatToInt(transform.position.x);
        int y = Block::floatToInt(transform.position.y);
        int z = Block::floatToInt(transform.position.z);

        // auto npos = transform.position + velocity;
        int nx = Block::floatToInt(npos.x);
        int ny = Block::floatToInt(npos.y);
        int nz = Block::floatToInt(npos.z);

        int minX = std::min(x, nx);
        int maxX = std::max(x, nx);
        int minY = std::min(y, ny);
        int maxY = std::max(y, ny);
        int minZ = std::min(z, nz);
        int maxZ = std::max(z, nz);

        int hitX = -1;
        int hitY = -1;
        int hitZ = -1;
        bool hit = false;

        for (int i = minX; i <= maxX; i++) {
                for (int j = minY; j <= maxY; j++) {
                        for (int k = minZ; k <= maxZ; k++) {
                                auto pt = glm::vec3(i, j, k);
                                if (!world.contains(pt) ||
                                    world.getBlockAt(i, j, k) != nullptr) {
                                        hit = true;
                                        hitX = i;
                                        hitY = j;
                                        hitZ = k;
                                }
                                if (hit) break;
                        }
                        if (hit) break;
                }
                if (hit) break;
        }
        (*outHit) = hit;
        return glm::vec3(hitX, hitY, hitZ);
}

void EntityPhysics::applyPhysics(World& world) {
        int groundY = Block::floatToInt(transform.position.y) - 1;
        if (groundY < 0) {
                groundY = 0;
        }
        this->_isGround = true;
        if (world.getBlockAt(Block::floatToInt(transform.position.x), groundY,
                             Block::floatToInt(transform.position.z)) ==
            nullptr) {
                this->velocity.y -= 0.1f;
                this->_isGround = false;
        }
        auto newPos = transform.position;
        auto xNewPos = transform.position + (glm::vec3(1, 0, 0) * velocity);
        bool isXHit = false;
        auto hitX = getHitBlock(world, xNewPos, &isXHit);
        if (!isXHit) {
                newPos.x = xNewPos.x;
        } else {
                if (velocity.x > 0) {
                        newPos.x = hitX.x - 1;
                } else {
                        newPos.x = hitX.x + 1;
                }
                velocity.x = 0;
        }
        auto yNewPos = transform.position + (glm::vec3(0, 1, 0) * velocity);
        bool isYHit = false;
        auto hitY = getHitBlock(world, yNewPos, &isYHit);
        if (!isYHit) {
                newPos.y = yNewPos.y;
        } else {
                if (velocity.y > 0) {
                        newPos.y = hitY.y - 1;
                } else {
                        newPos.y = hitY.y + 1;
                }
                velocity.y = 0;
        }
        auto zNewPos = transform.position + (glm::vec3(0, 0, 1) * velocity);
        bool isZHit = false;
        auto hitZ = getHitBlock(world, zNewPos, &isZHit);
        if (!isZHit) {
                newPos.z = zNewPos.z;
        } else {
                if (velocity.z > 0) {
                        newPos.z = hitZ.z - 1;
                } else {
                        newPos.z = hitZ.z + 1;
                }
                velocity.z = 0;
        }
        transform.position = newPos;
}

bool EntityPhysics::isGround() const { return _isGround; }