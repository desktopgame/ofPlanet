#ifndef GAME_WORLD_GEN_CELL_HPP
#define GAME_WORLD_GEN_CELL_HPP
namespace planet {
struct Cell {
        int x;
        int z;
        float noise;
        Cell(int x, int z, float noise);
};
}  // namespace planet
#endif