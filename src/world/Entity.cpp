#include "Entity.hpp"
namespace planet {
const std::string Entity::TAG = "Entity";
Entity::Entity(std::weak_ptr<World> worldRef)
    : destroyed(false), transform(), worldRef(worldRef) {}

void Entity::update() {}

void Entity::draw() {}

void Entity::destroy() {
        if (!this->isDestroyed()) {
                this->destroyed = true;
                onDestroy();
        }
}

bool Entity::isDestroyed() const { return destroyed; }

Transform& Entity::getTransform() { return transform; }

const Transform& Entity::getTransform() const { return transform; }

std::shared_ptr<World> Entity::getWorld() const { return worldRef.lock(); }

std::string Entity::getTag() const {
	return Entity::TAG;
}

void Entity::onDestroy() {}
}  // namespace planet
