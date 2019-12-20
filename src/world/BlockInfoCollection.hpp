#pragma once
#ifndef WORLD_BLOCKINFOCOLLECTION_HPP
#define WORLD_BLOCKINFOCOLLECTION_HPP
#include <string>
#include <vector>
#include "mapbox/optional.hpp"

namespace planet {
struct BlockInfo {
	explicit BlockInfo() = default;
	std::string textue;
	std::string reference;
};

class BlockInfoCollection {
public:
	explicit BlockInfoCollection();
	std::string serialize() const;
	void deserialize(const std::string& json);

	void addBlockInfo(const BlockInfo& blockInfo);
	BlockInfo getBlockInfo(int index) const;
	mapbox::util::optional<BlockInfo> getBlockInfo(const std::string& reference) const;
	int getBlockInfoCount() const;

private:
	std::vector<BlockInfo> blockInfoVec;
};
}
#endif