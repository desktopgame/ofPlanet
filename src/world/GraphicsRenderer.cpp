#include "GraphicsRenderer.hpp"
#include "GraphicsBatch.hpp"

namespace planet {
GraphicsRenderer::GraphicsRenderer(ofShader & shader)
 : shader(shader), map() {
}
void GraphicsRenderer::putFront(GLuint texture, float x, float y, float z) {
	ref(texture)->putFront(x, y, z);
}
void GraphicsRenderer::putBack(GLuint texture, float x, float y, float z) {
	ref(texture)->putBack(x, y, z);
}
void GraphicsRenderer::putLeft(GLuint texture, float x, float y, float z) {
	ref(texture)->putLeft(x, y, z);
}
void GraphicsRenderer::putRight(GLuint texture, float x, float y, float z) {
	ref(texture)->putRight(x, y, z);
}
void GraphicsRenderer::putTop(GLuint texture, float x, float y, float z) {
	ref(texture)->putTop(x, y, z);
}
void GraphicsRenderer::putBottom(GLuint texture, float x, float y, float z) {
	ref(texture)->putBottom(x, y, z);
}
void GraphicsRenderer::clear() {
	for (auto& kv : map) {
		kv.second->clear();
	}
}
void GraphicsRenderer::update() {
	for (auto& kv : map) {
		kv.second->update();
	}
}
void GraphicsRenderer::render() {
	for (auto& kv : map) {
		kv.second->render(kv.first);
	}
}
std::shared_ptr<GraphicsBatch> GraphicsRenderer::ref(GLuint texture) {
	if (!map.count(texture)) {
		map.insert_or_assign(texture, createBatch(this->shader));
	}
	return map.at(texture);
}
}