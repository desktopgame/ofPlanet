#include "CubeRenderer.hpp"
namespace planet {

CubeRenderer::CubeRenderer(const World& world, ofShader& shader, const glm::vec3& size)
    : GraphicsRenderer(world, shader), size(size) {}

std::shared_ptr<GraphicsBatch> CubeRenderer::createBatch(const World& world, ofShader & shader) {
	return std::make_shared<CubeBatch>(world, shader, size);
}
}  // namespace planet
