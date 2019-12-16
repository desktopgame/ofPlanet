#pragma once
#ifndef WORLD_WORLDIO_HPP
#define WORLD_WORLDIO_HPP
#include <string>
#include <memory>

namespace planet {
class World;

class WorldIO {
public:
	static void toJson(const std::string& outputPath, const std::shared_ptr<World>& world);
	static void toObj(const std::string& outputPath, const std::shared_ptr<World>& world);
private:
	WorldIO() = delete;
	~WorldIO() = delete;
};
}
#endif