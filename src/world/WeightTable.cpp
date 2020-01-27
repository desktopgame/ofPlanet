#include "WeightTable.hpp"
namespace planet {
WeightTable::WeightTable(int xSize, int ySize, int zSize) : xSize(xSize), ySize(ySize), zSize(zSize), vec() {
	for (int i = 0; i < xSize; i++) {
		std::vector<std::vector<int> > gridYZ;
		for (int j = 0; j < ySize; j++) {
			std::vector<int> lineZ;
			for (int k = 0; k < zSize; k++) {
				lineZ.emplace_back(0);
			}
			gridYZ.emplace_back(lineZ);
		}
		vec.emplace_back(gridYZ);
	}
}

void WeightTable::setWeight(int x, int y, int z, int weight) {
	vec[x][y][z] = weight;
}
int WeightTable::getWeight(int x, int y, int z) {
	return vec[x][y][z];
}
int WeightTable::getXSize() const {
	return xSize;
}
int WeightTable::getYSize() const {
	return ySize;
}
int WeightTable::getZSize() const {
	return zSize;
}
}