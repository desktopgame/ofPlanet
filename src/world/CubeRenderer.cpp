#include "CubeRenderer.hpp"
namespace planet {

CubeRenderer::CubeRenderer(ofShader& shader, const glm::vec3& size)
    : GraphicsRenderer(shader), size(size) {}

std::shared_ptr<GraphicsBatch> CubeRenderer::createBatch(ofShader & shader) {
	return std::make_shared<CubeBatch>(shader, size);
}
}  // namespace planet
