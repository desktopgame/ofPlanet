#include "BlockTable.hpp"

#include <cassert>

#include "MultiBlock.hpp"
#include "BlockPack.hpp"
namespace planet {

// BlockPrefab
BlockPrefab::BlockPrefab(int id, bool instanced)
    : id(id), instanced(instanced) {}

BlockPrefab::BlockPrefab() : id(-1), instanced(false) {}
// BlockTable

BlockTable::BlockTable(int xSize, int ySize, int zSize)
    : xSize(xSize), ySize(ySize), zSize(zSize), terrain() {
        for (int x = 0; x < xSize; x++) {
                std::vector<std::vector<BlockPrefab> > xline;
                for (int y = 0; y < ySize; y++) {
                        std::vector<BlockPrefab> yline;
                        for (int z = 0; z < zSize; z++) {
                                yline.emplace_back(BlockPrefab());
                        }
                        xline.emplace_back(yline);
                }
                vec.emplace_back(xline);
        }
}

BlockTable::BlockTable() : xSize(-1), ySize(-1), zSize(-1), terrain() {}

void BlockTable::set(int x, int y, int z, const BlockPrefab& block) {
        vec[x][y][z] = block;
}

BlockPrefab& BlockTable::get(int x, int y, int z) { return vec[x][y][z]; }

const BlockPrefab& BlockTable::get(int x, int y, int z) const {
        return vec.at(x).at(y).at(z);
}

void BlockTable::expand(int baseX, int baseY, int baseZ, const MultiBlock& mb) {
		auto data = toCellVec(mb);
        for (auto e : data) {
                int x = baseX + e.point.x;
                int y = baseY + e.point.y;
                int z = baseZ + e.point.z;
				int id = e.blockId;
                if (x >= xSize || y >= ySize || z >= zSize) {
                        continue;
                }
                if (id < 0) {
                        set(x, y, z, BlockPrefab(-1, false));
                } else {
                        set(x, y, z, BlockPrefab(id, false));
                }
        }
}

int BlockTable::getXSize() const { return xSize; }

int BlockTable::getYSize() const { return ySize; }

int BlockTable::getZSize() const { return zSize; }
void BlockTable::setTerrain(const Terrain terrain) { this->terrain = terrain; }
Terrain BlockTable::getTerrain() const { return terrain; }
}  // namespace planet
