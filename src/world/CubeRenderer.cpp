#include "CubeRenderer.hpp"
namespace planet {

CubeRenderer::CubeRenderer(ofShader& shader)
    : shader(shader), map() {}

void CubeRenderer::putFront(GLuint texture, float x, float y, float z) {
        ref(texture)->putFront(x, y, z);
}

void CubeRenderer::putBack(GLuint texture, float x, float y, float z) {
        ref(texture)->putBack(x, y, z);
}

void CubeRenderer::putLeft(GLuint texture, float x, float y, float z) {
        ref(texture)->putLeft(x, y, z);
}

void CubeRenderer::putRight(GLuint texture, float x, float y, float z) {
        ref(texture)->putRight(x, y, z);
}

void CubeRenderer::putTop(GLuint texture, float x, float y, float z) {
        ref(texture)->putTop(x, y, z);
}

void CubeRenderer::putBottom(GLuint texture, float x, float y, float z) {
        ref(texture)->putBottom(x, y, z);
}

void CubeRenderer::clear() {
        for (auto& kv : map) {
                kv.second->clear();
        }
}

void CubeRenderer::update() {
        for (auto& kv : map) {
                kv.second->update();
        }
}

void CubeRenderer::render() {
        for (auto& kv : map) {
                kv.second->render(kv.first);
        }
}

// private

//void CubeRenderer::put(GLuint texture, PlaneType type, float x, float y,
//	float z) {
//	ref(texture)->put(type, x, y, z);
//}


std::shared_ptr<CubeBatch> CubeRenderer::ref(GLuint texture) {
        if (map.count(texture)) {
                return map.at(texture);
        }
        auto r = std::make_shared<CubeBatch>(shader);
        map.insert_or_assign(texture, r);
        return r;
}
}  // namespace planet
