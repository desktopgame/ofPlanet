#include "EntityRigidbody.hpp"

#include <algorithm>
#include <cmath>

#include "World.hpp"
namespace planet {

EntityRigidbody::EntityRigidbody(std::weak_ptr<World> worldRef)
    : Entity(worldRef),
      velocity(),
      bIsGround(false),
      jumpTriggered(false),
      jumpVelocity(0) {}

void EntityRigidbody::update() { computePhysics(); }

void EntityRigidbody::startJump() {
        if (!bIsGround) {
                return;
        }
        this->jumpTriggered = true;
        this->jumpVelocity = 0;
}

void EntityRigidbody::computePhysics() {
        auto world = getWorld();
        // apply jump
        if (jumpTriggered) {
                this->jumpVelocity += 0.1f;
                this->velocity.y += jumpVelocity;
                if (jumpVelocity >= 0.3f) {
                        this->jumpTriggered = false;
                }
        }
        // apply gravity, if block not exist
        int groundY = World::floatToInt(transform.position.y) - 1;
        if (groundY < 0) {
                groundY = 0;
        }
        this->bIsGround = true;
        int tx = World::floatToInt(transform.position.x);
        int ty = groundY;
        int tz = World::floatToInt(transform.position.z);
        if (world->isContains(tx, ty, tz) &&
            world->getBlockBehavior(tx, ty, tz) == nullptr) {
                this->velocity.y -= 0.1f;
                this->bIsGround = false;
        }
        auto oldPos = transform.position;
        auto newPos = transform.position;
        auto xNewPos = transform.position + (glm::vec3(1, 0, 0) * velocity);
        bool isXHit = false;
        glm::vec3 hitX;
        // auto hitX = getHitBlock(world, xNewPos, &isXHit);
        if (!world->raycast(oldPos, xNewPos, hitX)) {
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
        glm::vec3 hitY;
        // auto hitY = getHitBlock(world, yNewPos, &isYHit);
        if (!world->raycast(oldPos, yNewPos, hitY)) {
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
        glm::vec3 hitZ;
        // auto hitZ = getHitBlock(world, zNewPos, &isZHit);
        if (!world->raycast(oldPos, zNewPos, hitZ)) {
                newPos.z = zNewPos.z;
        } else {
                if (velocity.z > 0) {
                        newPos.z = hitZ.z - 1;
                } else {
                        newPos.z = hitZ.z + 1;
                }
                velocity.z = 0;
        }
        newPos.x = std::max(
            0.0f, std::min(static_cast<float>(world->getXSize()), newPos.x));
        newPos.y = std::max(
            0.0f, std::min(static_cast<float>(world->getYSize()), newPos.y));
        newPos.z = std::max(
            0.0f, std::min(static_cast<float>(world->getZSize()), newPos.z));
        transform.position = newPos;
}
}  // namespace planet
