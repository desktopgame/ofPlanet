#pragma once
#ifndef WORLD_WORLDIO_HPP
#define WORLD_WORLDIO_HPP
#include <string>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "../objb/ObjBuilder.hpp"
#include "../objb/MtlBuilder.hpp"

namespace planet {
class World;
class Planet;

class WorldIO {
public:
	static void toJson(const std::string& outputPath, const std::shared_ptr<World>& world);
	static void toObj(const std::string& outputPath, const std::shared_ptr<World>& world);
	static void toBmp(const std::string& outputPath, const std::shared_ptr<Planet>& planet);
private:
	static void genTopPlane(objb::ObjBuilder& ob, objb::MtlBuilder& mb, int x, int y, int z, glm::vec3 size, const std::shared_ptr<World>& world);
	static void genBottomPlane(objb::ObjBuilder& ob, objb::MtlBuilder& mb, int x, int y, int z, glm::vec3 size, const std::shared_ptr<World>& world);
	static void genLeftPlane(objb::ObjBuilder& ob, objb::MtlBuilder& mb, int x, int y, int z, glm::vec3 size, const std::shared_ptr<World>& world);
	static void genRightPlane(objb::ObjBuilder& ob, objb::MtlBuilder& mb, int x, int y, int z, glm::vec3 size, const std::shared_ptr<World>& world);
	static void genFrontPlane(objb::ObjBuilder& ob, objb::MtlBuilder& mb, int x, int y, int z, glm::vec3 size, const std::shared_ptr<World>& world);
	static void genBackPlane(objb::ObjBuilder& ob, objb::MtlBuilder& mb, int x, int y, int z, glm::vec3 size, const std::shared_ptr<World>& world);

	WorldIO() = delete;
	~WorldIO() = delete;
};
}
#endif