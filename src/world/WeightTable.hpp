#pragma once
#ifndef WORLD_WEIGHTTABLE_HPP
#define WORLD_WEIGHTTABLE_HPP
#include <vector>

namespace planet {
class WeightTable {
public:
	explicit WeightTable(int xSize, int ySize, int zSize);

	void setWeight(int x, int y, int z, int weight);
	int getWeight(int x, int y, int z);

	int getXSize() const;
	int getYSize() const;
	int getZSize() const;
private:
	int xSize, ySize, zSize;
	std::vector<std::vector<std::vector<int > > > vec;
};
}
#endif