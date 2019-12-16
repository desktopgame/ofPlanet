#include "BlockTable.hpp"

#include <cassert>

#include "MultiBlock.hpp"
namespace planet {

// BlockPrefab
BlockPrefab::BlockPrefab(int id, bool instanced) : id(id), instanced(instanced) {}

BlockPrefab::BlockPrefab() : id(-1), instanced(false) {}
// BlockTable

BlockTable::BlockTable(int xSize, int ySize, int zSize)
    : xSize(xSize), ySize(ySize), zSize(zSize) {
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

BlockTable::BlockTable() : xSize(-1), ySize(-1), zSize(-1) {}

void BlockTable::set(int x, int y, int z, const BlockPrefab& block) {
        vec[x][y][z] = block;
}

BlockPrefab& BlockTable::get(int x, int y, int z) { return vec[x][y][z]; }

const BlockPrefab& BlockTable::get(int x, int y, int z) const {
        return vec.at(x).at(y).at(z);
}

void BlockTable::expand(int baseX, int baseY, int baseZ, const MultiBlock& mb) {
        auto data = mb.to3DData();
        for (auto e : data) {
                int x = baseX + (int)e.first.x;
                int y = baseY + (int)e.first.y;
                int z = baseZ + (int)e.first.z;
                int id = mb.decode(e.second);
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
}  // namespace planet
