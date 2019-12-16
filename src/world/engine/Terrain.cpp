#include "Terrain.hpp"
namespace planet {
Terrain::Terrain() : cells(), pockets() {
}
void Terrain::addCell(const Cell & cell) {
	cells.emplace_back(cell);
}
Cell Terrain::getCellAt(int index) const {
	return cells.at(index);
}
int Terrain::getCellCount() const {
	return static_cast<int>(cells.size());
}
void Terrain::addPocket(const Pocket & pocket) {
	pockets.emplace_back(pocket);
}
Pocket Terrain::getPocketAt(int index) const {
	return pockets.at(index);
}
int Terrain::getPocketCount() const {
	return static_cast<int>(pockets.size());
}
}