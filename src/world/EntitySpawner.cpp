#include "EntitySpawner.hpp"
#include "World.hpp"
#include <ofAppRunner.h>
namespace planet {
	EntitySpawner::EntitySpawner(std::weak_ptr<World> worldRef, float seconds, SpawnFn fn)
	 : Entity(worldRef), seconds(seconds), fn(fn), elapsed(0) {
		float now = static_cast<float>(ofGetElapsedTimef());
		this->time = now;
	}
	void EntitySpawner::update()
	{
	}
	void EntitySpawner::draw()
	{
		float now = static_cast<float>(ofGetElapsedTimef());
		float delta = (now - time);
		this->time = now;
		this->elapsed += delta;
		if (elapsed >= seconds) {
			this->elapsed = 0;
			spawn();
		}
	}
	void EntitySpawner::spawn()
	{
		fn(getWorld());
	}
}