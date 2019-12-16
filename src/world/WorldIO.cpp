#include "WorldIO.hpp"
#include <sstream>
#include "World.hpp"
#include "BlockPack.hpp"
#include "TexturePack.hpp"
#include "Block.hpp"
#include "../io/File.hpp"
#include "../picojson/picojson.h"

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
					picojson::object blockO;
					blockO["x"] = picojson::value(static_cast<double>(x));
					blockO["y"] = picojson::value(static_cast<double>(y));
					blockO["z"] = picojson::value(static_cast<double>(z));
					blockO["texture"] = picojson::value(block->getTextureSet().getName());
					blocksA.emplace_back(blockO);
				}
			}
		}
	}
	worldSizeO["x"] = picojson::value(static_cast<double>(world->getXSize()));
	worldSizeO["y"] = picojson::value(static_cast<double>(world->getYSize()));
	worldSizeO["z"] = picojson::value(static_cast<double>(world->getZSize()));
	rootO["blocks"] = picojson::value(blocksA);
	rootO["worldSize"] = picojson::value(worldSizeO);
	ss << picojson::value(rootO) << std::endl;
	File::writeAllText(outputPath, ss.str());
}
void WorldIO::toObj(const std::string & outputPath, const std::shared_ptr<World>& world) {
}
}