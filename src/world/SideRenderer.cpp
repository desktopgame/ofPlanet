#include "SideRenderer.hpp"

#include "../common/GLM.hpp"
namespace planet {

#define Q_I (glm::mat4(1.0f))

SideRenderer::SideRenderer(ofShader& shader)
    : shader(shader), isInvalid(true), planes(), posVec(), vbo() {
        for (int i = 0; i < static_cast<int>(PlaneType::Count); i++) {
                std::vector<float> v;
                planes[i] = std::make_shared<Plane>(shader, static_cast<PlaneType>(i),
                                                    glm::vec3(1, 1, 1));
                posVec[i] = v;
                vbo[i].allocate();
        }
}

SideRenderer::~SideRenderer() {}

void SideRenderer::put(PlaneType type, float x, float y, float z) {
        getPosVec(type).emplace_back(x);
        getPosVec(type).emplace_back(y);
        getPosVec(type).emplace_back(z);
        this->isInvalid = true;
}

void SideRenderer::putFront(float x, float y, float z) {
        put(PlaneType::Front, x, y, z);
}

void SideRenderer::putBack(float x, float y, float z) {
        put(PlaneType::Back, x, y, z);
}

void SideRenderer::putLeft(float x, float y, float z) {
        put(PlaneType::Left, x, y, z);
}

void SideRenderer::putRight(float x, float y, float z) {
        put(PlaneType::Right, x, y, z);
}

void SideRenderer::putTop(float x, float y, float z) {
        put(PlaneType::Top, x, y, z);
}

void SideRenderer::putBottom(float x, float y, float z) {
        put(PlaneType::Bottom, x, y, z);
}

void SideRenderer::clear() {
        for (int i = 0; i < static_cast<int>(PlaneType::Count); i++) {
                posVec.at(i).clear();
        }
        this->isInvalid = true;
}

void SideRenderer::updatePlane() {
        if (!isInvalid) {
                return;
        }
        for (int i = 0; i < static_cast<int>(PlaneType::Count); i++) {
                updatePlane(static_cast<PlaneType>(i));
        }
        this->isInvalid = false;
}

void SideRenderer::render(GLuint texture) {
		updatePlane();
        glBindTexture(GL_TEXTURE_2D, texture);

        for (int i = 0; i < static_cast<int>(PlaneType::Count); i++) {
                planes[i]->drawInstanced(
                    getPosVec(static_cast<PlaneType>(i)).size());
        }
}

void SideRenderer::updatePlane(PlaneType type) {
        int index = static_cast<int>(type);
        std::vector<float>& posVec = getPosVec(type);
        if (posVec.empty()) {
                return;
        }
        // update vbo
        ofBufferObject& v = vbo[index];
        v.bind(GL_ARRAY_BUFFER);
        v.setData(posVec, GL_STATIC_DRAW);
        v.unbind(GL_ARRAY_BUFFER);
        // update vao
        ofVbo& vao = planes[index]->getVAO();
		shader.begin();
        vao.bind();
        // vertex Attributes
        vao.setAttributeBuffer(4, v, 3, 0);
        vao.setAttributeDivisor(0, 0);
        vao.setAttributeDivisor(2, 0);
        vao.setAttributeDivisor(3, 0);
        vao.setAttributeDivisor(4, 1);

        vao.unbind();
		shader.end();
}

std::vector<float>& SideRenderer::getPosVec(PlaneType type) {
        int i = static_cast<int>(type);
        assert(i >= 0 && i < static_cast<int>(PlaneType::Count));
        return posVec.at(i);
}
}  // namespace planet
