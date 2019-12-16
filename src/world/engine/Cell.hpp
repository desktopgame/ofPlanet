#ifndef GAME_WORLD_GEN_CELL_HPP
#define GAME_WORLD_GEN_CELL_HPP
namespace planet {
struct Cell {
        const int x;
        const int z;
        const float noise;
        Cell(int x, int z, float noise);
};
}  // namespace planet
#endif