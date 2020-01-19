#pragma once
#ifndef WORLD_WORLDIO_HPP
#define WORLD_WORLDIO_HPP
#include <cmath>
#include <string>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "../objb/ObjBuilder.hpp"
#include "../objb/MtlBuilder.hpp"

namespace planet {
class World;
class Planet;

class Progress {
public:
	explicit Progress();

	void setValue(float value);
	float getValue() const;
	bool isDone() const;
private:
	float value;
};
using ReadonlyProgress = const Progress;
using AsyncOperation = std::shared_ptr<ReadonlyProgress>;

class WorldIO {
public:
	static AsyncOperation toJson(const std::string& outputPath, const std::shared_ptr<World>& world);
	static AsyncOperation toObj(const std::string& outputPath, const std::shared_ptr<World>& world);
	static AsyncOperation toBmp(const std::string& outputPath, const std::shared_ptr<Planet>& planet);
private:
	template<typename T>
	static float onef(T a) { return std::max(1.0f, static_cast<float>(a)); }

	template<typename T>
	static float sumf(T a) { return static_cast<float>(onef(a)); }
	
	template<typename T>
	static float sumf(T a, T b) { return static_cast<float>(onef(a) * onef(b)); }

	template<typename T>
	static float sumf(T a, T b, T c) { return static_cast<float>(onef(a) * onef(b) * onef(c)); }

	static glm::vec3 asVec3(int x, int y, int z);

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