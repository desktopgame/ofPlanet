#include "CubeRenderer.hpp"
namespace planet {

CubeRenderer::CubeRenderer(ofShader& shader)
    : GraphicsRenderer(shader) {}

std::shared_ptr<GraphicsBatch> CubeRenderer::createBatch(ofShader & shader) {
	return std::make_shared<CubeBatch>(shader, glm::vec3(1,1,1));
}
}  // namespace planet
