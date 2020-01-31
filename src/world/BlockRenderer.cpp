#include "BlockRenderer.hpp"
namespace planet {
BlockRenderer::BlockRenderer(const World& world, ofShader & shader)
 : cubeRenderer(world, shader, glm::vec3(1,1,1)),
   horizontalSlabRenderer(world, shader, glm::vec3(1,1,0.5f)),
   verticalSlabRenderer(world, shader, glm::vec3(1,0.5f,1)) {
}
void BlockRenderer::clear() {
	cubeRenderer.clear();
}
void BlockRenderer::update() {
	cubeRenderer.update();
}
void BlockRenderer::render() {
	cubeRenderer.render();
}
CubeRenderer & BlockRenderer::getCubeRenderer() {
	return cubeRenderer;
}
CubeRenderer & BlockRenderer::getHorizontalSlabRenderer() {
	return horizontalSlabRenderer;
}
CubeRenderer & BlockRenderer::getVerticalSlabRenderer() {
	return verticalSlabRenderer;
}
}