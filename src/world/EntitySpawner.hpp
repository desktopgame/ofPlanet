#pragma once
#ifndef WORLD_ENTITYSPAWNER_HPP
#define WORLD_ENTITYSPAWNER_HPP
#include "Entity.hpp"
#include <functional>

namespace planet {
class EntitySpawner : public Entity {
public:
	using SpawnFn = std::function<void(std::shared_ptr<World>)>;
	explicit EntitySpawner(std::weak_ptr<World> worldRef, float seconds, SpawnFn fn);

	void update() override;
	void draw() override;
protected:
	void spawn();
private:
	SpawnFn fn;
	float seconds;
	float time;
	float elapsed;
};
}
#endif