#include "BlockInfoCollection.hpp"

#include "../picojson/picojson.h"

namespace planet {
BlockInfoCollection::BlockInfoCollection() : blockInfoVec() {}
std::string BlockInfoCollection::serialize() const {
        picojson::object rootO;
        picojson::array blocksA;
        for (auto blockInfo : blockInfoVec) {
                picojson::object blockO;
                blockO["texture"] = picojson::value(blockInfo.textue);
                blockO["reference"] = picojson::value(blockInfo.reference);
                blocksA.push_back(picojson::value(blockO));
        }
        rootO["blocks"] = picojson::value(blocksA);
        return picojson::value(rootO).serialize(true);
}
void BlockInfoCollection::deserialize(const std::string& json) {
        picojson::value root;
        const std::string err = picojson::parse(root, json);
        if (err.empty() == false) {
                std::cerr << err << std::endl;
                return;
        }
        auto rootO = root.get<picojson::object>();
        auto blocksV = rootO["blocks"];
        auto blocksA = blocksV.get<picojson::array>();
        auto blocksIter = blocksA.begin();
        while (blocksIter != blocksA.end()) {
                BlockInfo bi;
                auto blockV = *blocksIter;
                auto blockO = blockV.get<picojson::object>();
                auto textureV = blockO["texture"];
                auto referenceV = blockO["reference"];
                bi.textue = textureV.get<std::string>();
                bi.reference = referenceV.get<std::string>();
                blockInfoVec.emplace_back(bi);
                ++blocksIter;
        }
}
void BlockInfoCollection::addBlockInfo(const BlockInfo& blockInfo) {
        blockInfoVec.emplace_back(blockInfo);
}
BlockInfo BlockInfoCollection::getBlockInfo(int index) const {
        return blockInfoVec.at(index);
}
mapbox::util::optional<BlockInfo> BlockInfoCollection::getBlockInfo(
    const std::string& reference) const {
        auto iter = blockInfoVec.begin();
        while (iter != blockInfoVec.end()) {
                auto v = *iter;
                if (v.reference == reference) {
                        return v;
                }
                ++iter;
        }
        return mapbox::util::optional<BlockInfo>();
}
int BlockInfoCollection::getBlockInfoCount() const {
        return static_cast<int>(blockInfoVec.size());
}
}  // namespace planet