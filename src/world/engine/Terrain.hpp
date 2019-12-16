#pragma once
#ifndef WORLD_ENGINE_TERRAIN_HPP
#define WORLD_ENGINE_TERRAIN_HPP
#include <vector>
#include "Cell.hpp"
#include "Pocket.hpp"

namespace planet {
class Terrain {
	public:
		explicit Terrain();

		void addCell(const Cell& cell);
		Cell getCellAt(int index) const;
		int getCellCount() const;

		void addPocket(const Pocket& pocket);
		Pocket getPocketAt(int index) const;
		int getPocketCount() const;

	private:
		std::vector<Cell> cells;
		std::vector<Pocket> pockets;
};
}
#endif