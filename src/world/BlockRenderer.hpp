#pragma once
#ifndef WORLD_BLOCKRENDERER_HPP
#define WORLD_BLOCKRENDERER_HPP
#include "CubeRenderer.hpp"


namespace planet {
class BlockRenderer {
public:
	explicit BlockRenderer(ofShader& shader);

	void clear();
	void update();
	void render();
	CubeRenderer& getCubeRenderer();
private:
	CubeRenderer cubeRenderer;
};
}
#endif