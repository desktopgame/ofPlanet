#include "Planet.hpp"

#include "../common/Stopwatch.hpp"
#include "BlockTable.hpp"
#include "World.hpp"
#include "WorldGenerateService.hpp"
#include "biome/Biome.hpp"
namespace planet {

Planet::Planet(const NameSet& nameSet)
    : nameSet(nameSet), world(nullptr) {

}

void Planet::generate(glm::vec3 size, std::shared_ptr<Biome> biome) {
	this->world = World::create(nameSet, size);
	glm::ivec3 is = size;
	BlockTable bt = BlockTable(is.x, is.y, is.z);
	biome->generate(bt);
	world->load(bt);
}

void Planet::update() {
	if (world) {
		world->update();
	}
}

void Planet::drawToBuffer() {
	if (world) {
		world->drawToBuffer();
	}
}

void Planet::render() {
	if (world) {
		world->render();
	}
}

std::shared_ptr<World> Planet::getWorld() const { return world; }
}  // namespace planet
