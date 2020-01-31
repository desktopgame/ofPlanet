#include "CubeRenderer.hpp"
namespace planet {

CubeRenderer::CubeRenderer(ofShader& shader)
    : GraphicsRenderer(shader) {}

std::shared_ptr<GraphicsBatch> CubeRenderer::createBatch(ofShader & shader) {
	return std::make_shared<CubeBatch>(shader);
}
}  // namespace planet
