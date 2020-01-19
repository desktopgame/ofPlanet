#include "WorldIO.hpp"
#include <sstream>
#include "World.hpp"
#include "Planet.hpp"
#include "BlockPack.hpp"
#include "TexturePack.hpp"
#include "Block.hpp"
#include "../io/File.hpp"
#include "../picojson/picojson.h"
#include <soil.h>

namespace planet {
void WorldIO::toJson(const std::string & outputPath, const std::shared_ptr<World>& world) {
	if (!world) {
		return;
	}
	picojson::object rootO;
	picojson::array blocksA;
	picojson::object worldSizeO;
	std::stringstream ss;
	int xs = world->getXSize();
	int ys = world->getYSize();
	int zs = world->getZSize();
	for (int x = 0; x < xs; x++) {
		for (int y = 0; y < ys; y++) {
			for (int z = 0; z < zs; z++) {
				bool overXP = x + 1 >= xs;
				bool overXN = x - 1 < 0;
				bool overYP = y + 1 >= ys;
				bool overYN = y - 1 < 0;
				bool overZP = z + 1 >= zs;
				bool overZN = z - 1 < 0;

				std::shared_ptr<Block> null = nullptr;
				auto blockXP = overXP ? null : world->getBlockBehavior(x + 1, y, z);
				auto blockXN = overXN ? null : world->getBlockBehavior(x - 1, y, z);
				auto blockYP = overYP ? null : world->getBlockBehavior(x, y + 1, z);
				auto blockYN = overYN ? null : world->getBlockBehavior(x, y - 1, z);
				auto blockZP = overZP ? null : world->getBlockBehavior(x, y, z + 1);
				auto blockZN = overZN ? null : world->getBlockBehavior(x, y, z - 1);
				auto blockB = world->getBlockBehavior(x, y, z);
				if (blockB == nullptr) {
					continue;
				}
				auto block = std::static_pointer_cast<Block>(blockB);
				if (blockXP && blockXN && blockYP && blockYN && blockZP && blockZN) {
				}
				else {
					picojson::object blockO;
					blockO["x"] = picojson::value(static_cast<double>(x));
					blockO["y"] = picojson::value(static_cast<double>(y));
					blockO["z"] = picojson::value(static_cast<double>(z));
					blockO["block"] = picojson::value(block->getName());
					blocksA.emplace_back(blockO);
				}
			}
		}
	}
	worldSizeO["x"] = picojson::value(static_cast<double>(world->getXSize()));
	worldSizeO["y"] = picojson::value(static_cast<double>(world->getYSize()));
	worldSizeO["z"] = picojson::value(static_cast<double>(world->getZSize()));
	rootO["cell"] = picojson::value(blocksA);
	rootO["worldSize"] = picojson::value(worldSizeO);
	ss << picojson::value(rootO) << std::endl;
	File::writeAllText(outputPath, ss.str());
}

void WorldIO::toObj(const std::string & outputPath, const std::shared_ptr<World>& world) {
	auto w = world;
	using namespace objb;
	ObjBuilder ob;
	MtlBuilder mb;
	ob
		.globalTexcoord(glm::vec2(0, 0))
		.globalTexcoord(glm::vec2(1, 0))
		.globalTexcoord(glm::vec2(1, 1))
		.globalTexcoord(glm::vec2(0, 1))
		//top
		.globalNormal(glm::vec3(0, 1, 0))
		//bottom
		.globalNormal(glm::vec3(0, 1, 0))
		//left
		.globalNormal(glm::vec3(-1, 0, 0))
		//right
		.globalNormal(glm::vec3(0, 1, 0))
		//front
		.globalNormal(glm::vec3(0, 1, 0))
		//back
		.globalNormal(glm::vec3(0, 0, 1))
		;
	int xsize = w->getXSize();
	int ysize = w->getYSize();
	int zsize = w->getZSize();
	xsize = zsize = 32;
	glm::vec3 size(1, 1, 1);
	ob.reserveModels(xsize * ysize * zsize);
	for (int x = 0; x < xsize; x++) {
		for (int y = 0; y < ysize; y++) {
			for (int z = 0; z < zsize; z++) {
				auto block = w->getBlockBehavior(x, y, z);
				if (!block) {
					continue;
				}
				bool overXP = x + 1 >= xsize;
				bool overYP = y + 1 >= ysize;
				bool overZP = z + 1 >= zsize;
				bool overXN = x - 1 < 0;
				bool overYN = y - 1 < 0;
				bool overZN = z - 1 < 0;

				bool hasBlockXP = overXP ? false : w->getBlockBehavior(x + 1, y, z) != nullptr;
				bool hasBlockYP = overYP ? false : w->getBlockBehavior(x, y + 1, z) != nullptr;
				bool hasBlockZP = overZP ? false : w->getBlockBehavior(x, y, z + 1) != nullptr;
				bool hasBlockXN = overXN ? false : w->getBlockBehavior(x - 1, y, z) != nullptr;
				bool hasBlockYN = overYN ? false : w->getBlockBehavior(x, y - 1, z) != nullptr;
				bool hasBlockZN = overZN ? false : w->getBlockBehavior(x, y, z - 1) != nullptr;
				bool hiddenBlock = hasBlockXP && hasBlockYP && hasBlockZP && hasBlockXN && hasBlockYN && hasBlockZN;
				if (hiddenBlock) {
					continue;
				}
				hasBlockXP = hasBlockYP = hasBlockZP = hasBlockXN = hasBlockYN = hasBlockZN = false;
				if (!hasBlockYP) {
					genTopPlane(ob, mb, x * 2, y * 2, z * 2, size, w);
				}
				if (!hasBlockYN) {
					genBottomPlane(ob, mb, x * 2, y * 2, z * 2, size, w);
				}
				if (!hasBlockXP) {
					genRightPlane(ob, mb, x * 2, y * 2, z * 2, size, w);
				}
				if (!hasBlockXN) {
					genLeftPlane(ob, mb, x * 2, y * 2, z * 2, size, w);
				}
				if (!hasBlockZP) {
					genFrontPlane(ob, mb, x * 2, y * 2, z * 2, size, w);
				}
				if (!hasBlockZN) {
					genBackPlane(ob, mb, x * 2, y * 2, z * 2, size, w);
				}
			}
		}
	}

	std::ofstream ofs(outputPath);
	if (!ofs.fail()) {
		ob.write(ofs);
	}
	ofs.close();
}

void WorldIO::toBmp(const std::string & outputPath, const std::shared_ptr<Planet>& planet) {
	auto w = planet->getWorld();
	BlockTable blockTable = planet->getBlockTable();
	Terrain terrain = blockTable.getTerrain();
	std::vector<unsigned char> pixelVec = terrain.toPixelVec();
	int err = SOIL_save_image(outputPath.c_str(), SOIL_SAVE_TYPE_BMP, w->getXSize(), w->getZSize(), 4, pixelVec.data());
	if (err == 0) {
		std::cout << "export failed." << std::endl;
	}
}

// private
void WorldIO::genTopPlane(objb::ObjBuilder & ob, objb::MtlBuilder& mb, int x, int y, int z, glm::vec3 size, const std::shared_ptr<World>& world) {
	using namespace objb;
	char buf[256];
	std::memset(buf, '\0', 256);
	std::sprintf(buf, "plane%d%d%d_Top", x, y, z);

	ObjFace face;

	ObjPolygon polyA(ObjIndex(1), ObjIndex(1, IndexMode::Global), ObjIndex(1, IndexMode::Global));
	ObjPolygon polyB(ObjIndex(2), ObjIndex(2, IndexMode::Global), ObjIndex(1, IndexMode::Global));
	ObjPolygon polyC(ObjIndex(4), ObjIndex(3, IndexMode::Global), ObjIndex(1, IndexMode::Global));
	ObjPolygon polyD(ObjIndex(3), ObjIndex(4, IndexMode::Global), ObjIndex(1, IndexMode::Global));

	auto& aa = ob.newModel(buf)
		.sharedVertex(glm::vec3(-size.x, 0, size.z) + glm::vec3(x, y, z), polyA)
		.sharedVertex(glm::vec3(size.x, 0, size.z) + glm::vec3(x, y, z), polyB)
		.sharedVertex(glm::vec3(-size.x, 0, -size.z) + glm::vec3(x, y, z), polyC)
		.sharedVertex(glm::vec3(size.x, 0, -size.z) + glm::vec3(x, y, z), polyD);
	//ok
	face.emplace_back(polyC);
	face.emplace_back(polyA);
	face.emplace_back(polyB);
	face.emplace_back(polyD);

	aa.face(face);
}

void WorldIO::genBottomPlane(objb::ObjBuilder & ob, objb::MtlBuilder& mb, int x, int y, int z, glm::vec3 size, const std::shared_ptr<World>& world) {
	using namespace objb;
	char buf[256];
	std::memset(buf, '\0', 256);
	std::sprintf(buf, "plane%d%d%d_Bottom", x, y, z);

	ObjFace face;

	ObjPolygon polyA(ObjIndex(1), ObjIndex(1, IndexMode::Global), ObjIndex(2, IndexMode::Global));
	ObjPolygon polyB(ObjIndex(2), ObjIndex(2, IndexMode::Global), ObjIndex(2, IndexMode::Global));
	ObjPolygon polyC(ObjIndex(4), ObjIndex(3, IndexMode::Global), ObjIndex(2, IndexMode::Global));
	ObjPolygon polyD(ObjIndex(3), ObjIndex(4, IndexMode::Global), ObjIndex(2, IndexMode::Global));

	auto& aa = ob.newModel(buf)
		.sharedVertex(glm::vec3(size.x, -2, size.z) + glm::vec3(x, y, z), polyA)
		.sharedVertex(glm::vec3(-size.x, -2, size.z) + glm::vec3(x, y, z), polyB)
		.sharedVertex(glm::vec3(size.x, -2, -size.z) + glm::vec3(x, y, z), polyC)
		.sharedVertex(glm::vec3(-size.x, -2, -size.z) + glm::vec3(x, y, z), polyD);
	//ok
	face.emplace_back(polyC);
	face.emplace_back(polyA);
	face.emplace_back(polyB);
	face.emplace_back(polyD);

	aa.face(face);
}

void WorldIO::genLeftPlane(objb::ObjBuilder & ob, objb::MtlBuilder& mb, int x, int y, int z, glm::vec3 size, const std::shared_ptr<World>& world) {
	using namespace objb;
	char buf[256];
	std::memset(buf, '\0', 256);
	std::sprintf(buf, "plane%d%d%d_Left", x, y, z);

	ObjFace face;

	ObjPolygon polyA(ObjIndex(1), ObjIndex(1, IndexMode::Global), ObjIndex(3, IndexMode::Global));
	ObjPolygon polyB(ObjIndex(2), ObjIndex(2, IndexMode::Global), ObjIndex(3, IndexMode::Global));
	ObjPolygon polyC(ObjIndex(4), ObjIndex(3, IndexMode::Global), ObjIndex(3, IndexMode::Global));
	ObjPolygon polyD(ObjIndex(3), ObjIndex(4, IndexMode::Global), ObjIndex(3, IndexMode::Global));

	auto& aa = ob.newModel(buf)
		.sharedVertex(glm::vec3(-size.x, 0, -size.z) + glm::vec3(x, y, z), polyA)
		.sharedVertex(glm::vec3(-size.x, -2, -size.z) + glm::vec3(x, y, z), polyB)
		.sharedVertex(glm::vec3(-size.x, 0, size.z) + glm::vec3(x, y, z), polyC)
		.sharedVertex(glm::vec3(-size.x, -2, size.z) + glm::vec3(x, y, z), polyD);

	//ok
	face.emplace_back(polyD);
	face.emplace_back(polyC);
	face.emplace_back(polyA);
	face.emplace_back(polyB);

	aa.face(face);
}

void WorldIO::genRightPlane(objb::ObjBuilder & ob, objb::MtlBuilder& mb, int x, int y, int z, glm::vec3 size, const std::shared_ptr<World>& world) {
	using namespace objb;
	char buf[256];
	std::memset(buf, '\0', 256);
	std::sprintf(buf, "plane%d%d%d_Right", x, y, z);

	ObjFace face;

	ObjPolygon polyA(ObjIndex(1), ObjIndex(1, IndexMode::Global), ObjIndex(4, IndexMode::Global));
	ObjPolygon polyB(ObjIndex(2), ObjIndex(2, IndexMode::Global), ObjIndex(4, IndexMode::Global));
	ObjPolygon polyC(ObjIndex(4), ObjIndex(3, IndexMode::Global), ObjIndex(4, IndexMode::Global));
	ObjPolygon polyD(ObjIndex(3), ObjIndex(4, IndexMode::Global), ObjIndex(4, IndexMode::Global));

	auto& aa = ob.newModel(buf)
		.sharedVertex(glm::vec3(size.x, 0, size.z) + glm::vec3(x, y, z), polyA)
		.sharedVertex(glm::vec3(size.x, -2, size.z) + glm::vec3(x, y, z), polyB)
		.sharedVertex(glm::vec3(size.x, 0, -size.z) + glm::vec3(x, y, z), polyC)
		.sharedVertex(glm::vec3(size.x, -2, -size.z) + glm::vec3(x, y, z), polyD);

	//ok
	face.emplace_back(polyD);
	face.emplace_back(polyC);
	face.emplace_back(polyA);
	face.emplace_back(polyB);

	aa.face(face);
}

void WorldIO::genFrontPlane(objb::ObjBuilder & ob, objb::MtlBuilder& mb, int x, int y, int z, glm::vec3 size, const std::shared_ptr<World>& world) {
	using namespace objb;
	char buf[256];
	std::memset(buf, '\0', 256);
	std::sprintf(buf, "plane%d%d%d_Front", x, y, z);

	ObjFace face;

	ObjPolygon polyA(ObjIndex(1), ObjIndex(1, IndexMode::Global), ObjIndex(5, IndexMode::Global));
	ObjPolygon polyB(ObjIndex(2), ObjIndex(2, IndexMode::Global), ObjIndex(5, IndexMode::Global));
	ObjPolygon polyC(ObjIndex(4), ObjIndex(3, IndexMode::Global), ObjIndex(5, IndexMode::Global));
	ObjPolygon polyD(ObjIndex(3), ObjIndex(4, IndexMode::Global), ObjIndex(5, IndexMode::Global));

	auto& aa = ob.newModel(buf)
		.sharedVertex(glm::vec3(size.x, 0, -size.z) + glm::vec3(x, y, z), polyA)
		.sharedVertex(glm::vec3(size.x, -2, -size.z) + glm::vec3(x, y, z), polyB)
		.sharedVertex(glm::vec3(-size.x, 0, -size.z) + glm::vec3(x, y, z), polyC)
		.sharedVertex(glm::vec3(-size.x, -2, -size.z) + glm::vec3(x, y, z), polyD);

	face.emplace_back(polyC);
	face.emplace_back(polyD);
	face.emplace_back(polyB);
	face.emplace_back(polyA);
	//dcab
	//dabc
	//bdac
	//dacb
	//cdab
	//bcda
	//cbda
	//cdba
	aa.face(face);
}

void WorldIO::genBackPlane(objb::ObjBuilder & ob, objb::MtlBuilder& mb, int x, int y, int z, glm::vec3 size, const std::shared_ptr<World>& world) {
	using namespace objb;
	char buf[256];
	std::memset(buf, '\0', 256);
	std::sprintf(buf, "plane%d%d%d_Back", x, y, z);

	ObjFace face;

	ObjPolygon polyA(ObjIndex(1), ObjIndex(1, IndexMode::Global), ObjIndex(6, IndexMode::Global));
	ObjPolygon polyB(ObjIndex(2), ObjIndex(2, IndexMode::Global), ObjIndex(6, IndexMode::Global));
	ObjPolygon polyC(ObjIndex(4), ObjIndex(3, IndexMode::Global), ObjIndex(6, IndexMode::Global));
	ObjPolygon polyD(ObjIndex(3), ObjIndex(4, IndexMode::Global), ObjIndex(6, IndexMode::Global));

	auto& aa = ob.newModel(buf)
		.sharedVertex(glm::vec3(-size.x, 0, size.z) + glm::vec3(x, y, z), polyA)
		.sharedVertex(glm::vec3(-size.x, -2, size.z) + glm::vec3(x, y, z), polyB)
		.sharedVertex(glm::vec3(size.x, 0, size.z) + glm::vec3(x, y, z), polyC)
		.sharedVertex(glm::vec3(size.x, -2, size.z) + glm::vec3(x, y, z), polyD);

	face.emplace_back(polyC);
	face.emplace_back(polyD);
	face.emplace_back(polyB);
	face.emplace_back(polyA);


	aa.face(face);
}
}