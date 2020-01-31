#include "BlockRenderer.hpp"
namespace planet {
BlockRenderer::BlockRenderer(ofShader & shader)
 : cubeRenderer(shader, glm::vec3(1,1,1)),
   horizontalSlabRenderer(shader, glm::vec3(1,1,0.5f)),
   verticalSlabRenderer(shader, glm::vec3(1,0.5f,1)) {
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