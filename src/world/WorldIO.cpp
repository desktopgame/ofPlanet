#include "WorldIO.hpp"
#include <sstream>
#include "World.hpp"
#include "Planet.hpp"
#include "BlockPack.hpp"
#include "TexturePack.hpp"
#include "Block.hpp"
#include "../io/File.hpp"
#include "../io/Path.hpp"
#include "../picojson/picojson.h"
#include <soil.h>
#include <future>
#include <thread>

namespace planet {
// Progress
Progress::Progress() : value(0.0f) {
}
void Progress::setValue(float value) {
	if (value < this->value) {
		value = this->value;
	}
	this->value = value;
}
float Progress::getValue() const {
	return this->value;
}
bool Progress::isDone() const {
	return this->value >= 1.0f;
}
// WorldIO
AsyncOperation WorldIO::toJson(const std::string & outputPath, const std::shared_ptr<World>& world) {
	auto ret = std::make_shared<Progress>();
	if (!world) {
		ret->setValue(1.0f);
		return ret;
	}
	std::thread([outputPath, world, ret]() -> void {
		picojson::object rootO;
		picojson::array blocksA;
		picojson::object worldSizeO;
		std::stringstream ss;
		int xs = world->getXSize();
		int ys = world->getYSize();
		int zs = world->getZSize();
		float all = static_cast<float>(xs * ys * zs);
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
						ret->setValue(sumf(x, y, z) / all);
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
					ret->setValue(sumf(x, y, z) / all);
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
		ret->setValue(1.0f);
	}).detach();
	return ret;
}

AsyncOperation WorldIO::toObj(const std::string & outputDir, const std::shared_ptr<World>& world) {
	auto outputPath = Path::build(std::vector<std::string>{outputDir, "data.obj"});
	auto ret = std::make_shared<Progress>();
	auto w = world;
	using namespace objb;
	std::thread([outputPath, w, ret, outputDir]() -> void {
		ObjBuilder ob;
		MtlBuilder mb;
		ob
			.globalTexcoord(glm::vec2(0, 0))
			.globalTexcoord(glm::vec2(1, 0))
			.globalTexcoord(glm::vec2(1, 1))
			.globalTexcoord(glm::vec2(0, 1))
			.globalNormal(glm::vec3(0, 1, 0))
			.globalNormal(glm::vec3(0, 1, 0))
			.globalNormal(glm::vec3(-1, 0, 0))
			.globalNormal(glm::vec3(0, 1, 0))
			.globalNormal(glm::vec3(0, 1, 0))
			.globalNormal(glm::vec3(0, 0, 1))
			;
		ob.material("data.obj.mtl");
		int xsize = w->getXSize();
		int ysize = w->getYSize();
		int zsize = w->getZSize();
		xsize = zsize = 32;
		float all = static_cast<float>(xsize * ysize * zsize);
		glm::vec3 size(1, 1, 1);
		ob.reserveModels(xsize * ysize * zsize);
		std::vector<std::string> texVec;
		for (int x = 0; x < xsize; x++) {
			for (int y = 0; y < ysize; y++) {
				for (int z = 0; z < zsize; z++) {
					auto block = w->getBlockBehavior(x, y, z);
					if (!block) {
						ret->setValue(sumf(x, y, z) / all);
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
						ret->setValue(sumf(x, y, z) / all);
						continue;
					}
					if (!hasBlockYP) {
						genTopPlane(outputDir, texVec, ob, mb, glm::ivec3(x,y,z), glm::ivec3(x * 2, y * 2, z * 2), size, w);
					}
					if (!hasBlockYN) {
						genBottomPlane(outputDir, texVec, ob, mb, glm::ivec3(x, y, z), glm::ivec3(x * 2, y * 2, z * 2), size, w);
					}
					if (!hasBlockXP) {
						genRightPlane(outputDir, texVec, ob, mb, glm::ivec3(x, y, z), glm::ivec3(x * 2, y * 2, z * 2), size, w);
					}
					if (!hasBlockXN) {
						genLeftPlane(outputDir, texVec, ob, mb, glm::ivec3(x, y, z), glm::ivec3(x * 2, y * 2, z * 2), size, w);
					}
					if (!hasBlockZP) {
						genFrontPlane(outputDir, texVec, ob, mb, glm::ivec3(x, y, z), glm::ivec3(x * 2, y * 2, z * 2), size, w);
					}
					if (!hasBlockZN) {
						genBackPlane(outputDir, texVec, ob, mb, glm::ivec3(x, y, z), glm::ivec3(x * 2, y * 2, z * 2), size, w);
					}
					ret->setValue(sumf(x,y,z) / all);
				}
			}
		}
		// OBJ‚ð•Û‘¶
		std::ofstream objOFS(outputPath);
		if (!objOFS.fail()) {
			ob.write(objOFS);
		}
		objOFS.close();
		// MTL‚ð•Û‘¶
		std::ofstream mtlOFS(outputPath + ".mtl");
		if (!objOFS.fail()) {
			mtlOFS << mb.toString() << std::endl;
		}
		mtlOFS.close();
		ret->setValue(1.0f);
	}).detach();
	return ret;
}

AsyncOperation WorldIO::toBmp(const std::string & outputPath, const std::shared_ptr<Planet>& planet) {
	auto ret = std::make_shared<Progress>();
	auto w = planet->getWorld();
	std::thread([outputPath, planet, w, ret]() -> void {
		BlockTable blockTable = planet->getBlockTable();
		Terrain terrain = blockTable.getTerrain();
		std::vector<unsigned char> pixelVec = terrain.toPixelVec();
		int err = SOIL_save_image(outputPath.c_str(), SOIL_SAVE_TYPE_BMP, w->getXSize(), w->getZSize(), 4, pixelVec.data());
		if (err == 0) {
			std::cout << "export failed." << std::endl;
		}
		ret->setValue(1.0f);
	}).detach();
	return ret;
}

glm::vec3 WorldIO::asVec3(int x, int y, int z) {
	return glm::vec3(x, y, z);
}

// private
void WorldIO::genTopPlane(const std::string& outputDir, std::vector<std::string>& texVec, objb::ObjBuilder & ob, objb::MtlBuilder& mb, glm::ivec3 worldPos, glm::ivec3 objPos, glm::vec3 size, const std::shared_ptr<World>& world) {
	using namespace objb;
	char buf[256];
	std::memset(buf, '\0', 256);
	std::sprintf(buf, "plane%d%d%d_Top", objPos.x, objPos.y, objPos.z);
	auto blockBeh = world->getBlockBehavior(worldPos.x, worldPos.y, worldPos.z);
	auto block = std::static_pointer_cast<Block>(blockBeh);
	auto texPath = block->getTextureSet().getTopImage()->getPath();
	auto texName = block->getTextureReference() + std::string("_Top");
	auto texFileName = Path::getFileNameFromPath(block->getTextureSet().getTopImage()->getPath());
	auto copyFile = Path::build(std::vector<std::string>{outputDir, texFileName});

	ObjFace face;

	ObjPolygon polyA(ObjIndex(1), ObjIndex(1, IndexMode::Global), ObjIndex(1, IndexMode::Global));
	ObjPolygon polyB(ObjIndex(2), ObjIndex(2, IndexMode::Global), ObjIndex(1, IndexMode::Global));
	ObjPolygon polyC(ObjIndex(4), ObjIndex(3, IndexMode::Global), ObjIndex(1, IndexMode::Global));
	ObjPolygon polyD(ObjIndex(3), ObjIndex(4, IndexMode::Global), ObjIndex(1, IndexMode::Global));

	auto& aa = ob.newModel(buf)
		.sharedVertex(glm::vec3(-size.x, 0, size.z) + asVec3(objPos.x, objPos.y, objPos.z), polyA)
		.sharedVertex(glm::vec3(size.x, 0, size.z) + asVec3(objPos.x, objPos.y, objPos.z), polyB)
		.sharedVertex(glm::vec3(-size.x, 0, -size.z) + asVec3(objPos.x, objPos.y, objPos.z), polyC)
		.sharedVertex(glm::vec3(size.x, 0, -size.z) + asVec3(objPos.x, objPos.y, objPos.z), polyD);

	if (std::find(texVec.begin(), texVec.end(), texName) == texVec.end()) {
		mb.newElement(texName).map_Kd(texFileName);
		texVec.emplace_back(texName);
		File::copy(texPath, copyFile);
	}
	aa.useMaterial(texName);

	face.emplace_back(polyC);
	face.emplace_back(polyA);
	face.emplace_back(polyB);
	face.emplace_back(polyD);

	aa.face(face);
}

void WorldIO::genBottomPlane(const std::string& outputDir, std::vector<std::string>& texVec, objb::ObjBuilder & ob, objb::MtlBuilder& mb, glm::ivec3 worldPos, glm::ivec3 objPos, glm::vec3 size, const std::shared_ptr<World>& world) {
	using namespace objb;
	char buf[256];
	std::memset(buf, '\0', 256);
	std::sprintf(buf, "plane%d%d%d_Bottom", objPos.x, objPos.y, objPos.z);
	auto blockBeh = world->getBlockBehavior(worldPos.x, worldPos.y, worldPos.z);
	auto block = std::static_pointer_cast<Block>(blockBeh);
	auto texPath = block->getTextureSet().getBottomImage()->getPath();
	auto texName = block->getTextureReference() + std::string("_Bottom");
	auto texFileName = Path::getFileNameFromPath(block->getTextureSet().getBottomImage()->getPath());
	auto copyFile = Path::build(std::vector<std::string>{outputDir, texFileName});

	ObjFace face;

	ObjPolygon polyA(ObjIndex(1), ObjIndex(1, IndexMode::Global), ObjIndex(2, IndexMode::Global));
	ObjPolygon polyB(ObjIndex(2), ObjIndex(2, IndexMode::Global), ObjIndex(2, IndexMode::Global));
	ObjPolygon polyC(ObjIndex(4), ObjIndex(3, IndexMode::Global), ObjIndex(2, IndexMode::Global));
	ObjPolygon polyD(ObjIndex(3), ObjIndex(4, IndexMode::Global), ObjIndex(2, IndexMode::Global));

	auto& aa = ob.newModel(buf)
		.sharedVertex(glm::vec3(size.x, -2, size.z) + asVec3(objPos.x, objPos.y, objPos.z), polyA)
		.sharedVertex(glm::vec3(-size.x, -2, size.z) + asVec3(objPos.x, objPos.y, objPos.z), polyB)
		.sharedVertex(glm::vec3(size.x, -2, -size.z) + asVec3(objPos.x, objPos.y, objPos.z), polyC)
		.sharedVertex(glm::vec3(-size.x, -2, -size.z) + asVec3(objPos.x, objPos.y, objPos.z), polyD);

	if (std::find(texVec.begin(), texVec.end(), texName) == texVec.end()) {
		mb.newElement(texName).map_Kd(texFileName);
		texVec.emplace_back(texName);
		File::copy(texPath, copyFile);
	}
	aa.useMaterial(texName);

	face.emplace_back(polyC);
	face.emplace_back(polyA);
	face.emplace_back(polyB);
	face.emplace_back(polyD);

	aa.face(face);
}

void WorldIO::genLeftPlane(const std::string& outputDir, std::vector<std::string>& texVec, objb::ObjBuilder & ob, objb::MtlBuilder& mb, glm::ivec3 worldPos, glm::ivec3 objPos, glm::vec3 size, const std::shared_ptr<World>& world) {
	using namespace objb;
	char buf[256];
	std::memset(buf, '\0', 256);
	std::sprintf(buf, "plane%d%d%d_Left", objPos.x, objPos.y, objPos.z);
	auto blockBeh = world->getBlockBehavior(worldPos.x, worldPos.y, worldPos.z);
	auto block = std::static_pointer_cast<Block>(blockBeh);
	auto texPath =block->getTextureSet().getLeftImage()->getPath();
	auto texName = block->getTextureReference() + std::string("_Left");
	auto texFileName = Path::getFileNameFromPath(block->getTextureSet().getLeftImage()->getPath());
	auto copyFile = Path::build(std::vector<std::string>{outputDir, texFileName});

	ObjFace face;

	ObjPolygon polyA(ObjIndex(1), ObjIndex(1, IndexMode::Global), ObjIndex(3, IndexMode::Global));
	ObjPolygon polyB(ObjIndex(2), ObjIndex(2, IndexMode::Global), ObjIndex(3, IndexMode::Global));
	ObjPolygon polyC(ObjIndex(4), ObjIndex(3, IndexMode::Global), ObjIndex(3, IndexMode::Global));
	ObjPolygon polyD(ObjIndex(3), ObjIndex(4, IndexMode::Global), ObjIndex(3, IndexMode::Global));

	auto& aa = ob.newModel(buf)
		.sharedVertex(glm::vec3(-size.x, 0, -size.z) + asVec3(objPos.x, objPos.y, objPos.z), polyA)
		.sharedVertex(glm::vec3(-size.x, -2, -size.z) + asVec3(objPos.x, objPos.y, objPos.z), polyB)
		.sharedVertex(glm::vec3(-size.x, 0, size.z) + asVec3(objPos.x, objPos.y, objPos.z), polyC)
		.sharedVertex(glm::vec3(-size.x, -2, size.z) + asVec3(objPos.x, objPos.y, objPos.z), polyD);

	if (std::find(texVec.begin(), texVec.end(), texName) == texVec.end()) {
		mb.newElement(texName).map_Kd(texFileName);
		texVec.emplace_back(texName);
		File::copy(texPath, copyFile);
	}
	aa.useMaterial(texName);


	face.emplace_back(polyD);
	face.emplace_back(polyC);
	face.emplace_back(polyA);
	face.emplace_back(polyB);

	aa.face(face);
}

void WorldIO::genRightPlane(const std::string& outputDir, std::vector<std::string>& texVec, objb::ObjBuilder & ob, objb::MtlBuilder& mb, glm::ivec3 worldPos, glm::ivec3 objPos, glm::vec3 size, const std::shared_ptr<World>& world) {
	using namespace objb;
	char buf[256];
	std::memset(buf, '\0', 256);
	std::sprintf(buf, "plane%d%d%d_Right", objPos.x, objPos.y, objPos.z);
	auto blockBeh = world->getBlockBehavior(worldPos.x, worldPos.y, worldPos.z);
	auto block = std::static_pointer_cast<Block>(blockBeh);
	auto texPath =  block->getTextureSet().getRightImage()->getPath();
	auto texName = block->getTextureReference() + std::string("_Right");
	auto texFileName = Path::getFileNameFromPath(block->getTextureSet().getRightImage()->getPath());
	auto copyFile = Path::build(std::vector<std::string>{outputDir, texFileName });

	ObjFace face;

	ObjPolygon polyA(ObjIndex(1), ObjIndex(1, IndexMode::Global), ObjIndex(4, IndexMode::Global));
	ObjPolygon polyB(ObjIndex(2), ObjIndex(2, IndexMode::Global), ObjIndex(4, IndexMode::Global));
	ObjPolygon polyC(ObjIndex(4), ObjIndex(3, IndexMode::Global), ObjIndex(4, IndexMode::Global));
	ObjPolygon polyD(ObjIndex(3), ObjIndex(4, IndexMode::Global), ObjIndex(4, IndexMode::Global));

	auto& aa = ob.newModel(buf)
		.sharedVertex(glm::vec3(size.x, 0, size.z) + asVec3(objPos.x, objPos.y, objPos.z), polyA)
		.sharedVertex(glm::vec3(size.x, -2, size.z) + asVec3(objPos.x, objPos.y, objPos.z), polyB)
		.sharedVertex(glm::vec3(size.x, 0, -size.z) + asVec3(objPos.x, objPos.y, objPos.z), polyC)
		.sharedVertex(glm::vec3(size.x, -2, -size.z) + asVec3(objPos.x, objPos.y, objPos.z), polyD);

	if (std::find(texVec.begin(), texVec.end(), texName) == texVec.end()) {
		mb.newElement(texName).map_Kd(texFileName);
		texVec.emplace_back(texName);
		File::copy(texPath, copyFile);
	}
	aa.useMaterial(texName);


	face.emplace_back(polyD);
	face.emplace_back(polyC);
	face.emplace_back(polyA);
	face.emplace_back(polyB);

	aa.face(face);
}

void WorldIO::genFrontPlane(const std::string& outputDir, std::vector<std::string>& texVec, objb::ObjBuilder & ob, objb::MtlBuilder& mb, glm::ivec3 worldPos, glm::ivec3 objPos, glm::vec3 size, const std::shared_ptr<World>& world) {
	using namespace objb;
	char buf[256];
	std::memset(buf, '\0', 256);
	std::sprintf(buf, "plane%d%d%d_Front", objPos.x, objPos.y, objPos.z);
	auto blockBeh = world->getBlockBehavior(worldPos.x, worldPos.y, worldPos.z);
	auto block = std::static_pointer_cast<Block>(blockBeh);
	auto texPath =  block->getTextureSet().getFrontImage()->getPath();
	auto texName = block->getTextureReference() + std::string("_Front");
	auto texFileName = Path::getFileNameFromPath(block->getTextureSet().getFrontImage()->getPath());
	auto copyFile = Path::build(std::vector<std::string>{outputDir, texFileName});

	ObjFace face;

	ObjPolygon polyA(ObjIndex(1), ObjIndex(1, IndexMode::Global), ObjIndex(6, IndexMode::Global));
	ObjPolygon polyB(ObjIndex(2), ObjIndex(2, IndexMode::Global), ObjIndex(6, IndexMode::Global));
	ObjPolygon polyC(ObjIndex(4), ObjIndex(3, IndexMode::Global), ObjIndex(6, IndexMode::Global));
	ObjPolygon polyD(ObjIndex(3), ObjIndex(4, IndexMode::Global), ObjIndex(6, IndexMode::Global));

	auto& aa = ob.newModel(buf)
		.sharedVertex(glm::vec3(-size.x, 0, size.z) + asVec3(objPos.x, objPos.y, objPos.z), polyA)
		.sharedVertex(glm::vec3(-size.x, -2, size.z) + asVec3(objPos.x, objPos.y, objPos.z), polyB)
		.sharedVertex(glm::vec3(size.x, 0, size.z) + asVec3(objPos.x, objPos.y, objPos.z), polyC)
		.sharedVertex(glm::vec3(size.x, -2, size.z) + asVec3(objPos.x, objPos.y, objPos.z), polyD);

	if (std::find(texVec.begin(), texVec.end(), texName) == texVec.end()) {
		mb.newElement(texName).map_Kd(texFileName);
		texVec.emplace_back(texName);
		File::copy(texPath, copyFile);
	}
	aa.useMaterial(texName);

	face.emplace_back(polyC);
	face.emplace_back(polyA);
	face.emplace_back(polyB);
	face.emplace_back(polyD);

	aa.face(face);
}

void WorldIO::genBackPlane(const std::string& outputDir, std::vector<std::string>& texVec, objb::ObjBuilder & ob, objb::MtlBuilder& mb, glm::ivec3 worldPos, glm::ivec3 objPos, glm::vec3 size, const std::shared_ptr<World>& world) {
	using namespace objb;
	char buf[256];
	std::memset(buf, '\0', 256);
	std::sprintf(buf, "plane%d%d%d_Back", objPos.x, objPos.y, objPos.z);
	auto blockBeh = world->getBlockBehavior(worldPos.x, worldPos.y, worldPos.z);
	auto block = std::static_pointer_cast<Block>(blockBeh);
	auto texPath = block->getTextureSet().getBackImage()->getPath();
	auto texName = block->getTextureReference() + std::string("_Back");
	auto texFileName = Path::getFileNameFromPath(block->getTextureSet().getBackImage()->getPath());
	auto copyFile = Path::build(std::vector<std::string>{outputDir, texFileName});

	ObjFace face;

	ObjPolygon polyA(ObjIndex(1), ObjIndex(1, IndexMode::Global), ObjIndex(5, IndexMode::Global));
	ObjPolygon polyB(ObjIndex(2), ObjIndex(2, IndexMode::Global), ObjIndex(5, IndexMode::Global));
	ObjPolygon polyC(ObjIndex(4), ObjIndex(3, IndexMode::Global), ObjIndex(5, IndexMode::Global));
	ObjPolygon polyD(ObjIndex(3), ObjIndex(4, IndexMode::Global), ObjIndex(5, IndexMode::Global));

	auto& aa = ob.newModel(buf)
		.sharedVertex(glm::vec3(size.x, 0, -size.z) + asVec3(objPos.x, objPos.y, objPos.z), polyA)
		.sharedVertex(glm::vec3(size.x, -2, -size.z) + asVec3(objPos.x, objPos.y, objPos.z), polyB)
		.sharedVertex(glm::vec3(-size.x, 0, -size.z) + asVec3(objPos.x, objPos.y, objPos.z), polyC)
		.sharedVertex(glm::vec3(-size.x, -2, -size.z) + asVec3(objPos.x, objPos.y, objPos.z), polyD);

	if (std::find(texVec.begin(), texVec.end(), texName) == texVec.end()) {
		mb.newElement(texName).map_Kd(texFileName);
		texVec.emplace_back(texName);
		File::copy(texPath, copyFile);
	}
	aa.useMaterial(texName);


	face.emplace_back(polyC);
	face.emplace_back(polyA);
	face.emplace_back(polyB);
	face.emplace_back(polyD);

	aa.face(face);
}
}