#include "Entity.hpp"
Entity::Entity() : transform(), _destroy(false) {}
void Entity::update(World& world) {}
void Entity::draw() {}

void Entity::destroy() { this->_destroy = true; }

bool Entity::isDestroyed() const { return _destroy; }