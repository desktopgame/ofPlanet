#pragma once
#ifndef WORLD_BLOCKRENDERER_HPP
#define WORLD_BLOCKRENDERER_HPP
#include "CubeRenderer.hpp"


namespace planet {
class World;
class BlockRenderer {
public:
	explicit BlockRenderer(const World& world, ofShader& shader);

	void clear();
	void update();
	void render();

	CubeRenderer& getCubeRenderer();
	CubeRenderer& getHorizontalSlabRenderer();
	CubeRenderer& getVerticalSlabRenderer();
private:
	CubeRenderer cubeRenderer;
	CubeRenderer horizontalSlabRenderer;
	CubeRenderer verticalSlabRenderer;
};
}
#endif