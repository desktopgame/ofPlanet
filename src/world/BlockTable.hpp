#pragma once
#ifndef WORLD_BLOCKTABLE_HPP
#define WORLD_BLOCKTABLE_HPP
#include <vector>
namespace planet {

class MultiBlock;

struct BlockPrefab {
        explicit BlockPrefab(int id, bool instanced);
        explicit BlockPrefab();
        int id;
        bool instanced;
};

class BlockTable {
       public:
        explicit BlockTable(int xSize, int ySize, int zSize);
        BlockTable();
        void set(int x, int y, int z, const BlockPrefab& block);
        BlockPrefab& get(int x, int y, int z);
        const BlockPrefab& get(int x, int y, int z) const;

        void expand(int baseX, int baseY, int baseZ, const MultiBlock& mb);

        int getXSize() const;
        int getYSize() const;
        int getZSize() const;

       private:
        int xSize, ySize, zSize;
        std::vector<std::vector<std::vector<BlockPrefab> > > vec;
};
}  // namespace planet
#endif