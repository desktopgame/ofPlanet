#include "CubeBatch.hpp"

#include "../common/GLM.hpp"
namespace planet {



CubeBatch::CubeBatch(ofShader& shader)
    : shader(shader), isInvalid(true), planes(), posVec(), vbo() {
        for (int i = 0; i < static_cast<int>(PlaneType::Count); i++) {
                std::vector<float> v;
                planes[i] = std::make_shared<Plane>(shader, static_cast<PlaneType>(i),
                                                    glm::vec3(1, 1, 1));
                posVec[i] = v;
                vbo[i].allocate();
        }
}

CubeBatch::~CubeBatch() {}

void CubeBatch::putFront(float x, float y, float z) {
        put(PlaneType::Front, x, y, z);
}

void CubeBatch::putBack(float x, float y, float z) {
        put(PlaneType::Back, x, y, z);
}

void CubeBatch::putLeft(float x, float y, float z) {
        put(PlaneType::Left, x, y, z);
}

void CubeBatch::putRight(float x, float y, float z) {
        put(PlaneType::Right, x, y, z);
}

void CubeBatch::putTop(float x, float y, float z) {
        put(PlaneType::Top, x, y, z);
}

void CubeBatch::putBottom(float x, float y, float z) {
        put(PlaneType::Bottom, x, y, z);
}

void CubeBatch::clear() {
        for (int i = 0; i < static_cast<int>(PlaneType::Count); i++) {
                posVec.at(i).clear();
        }
        this->isInvalid = true;
}

void CubeBatch::update() {
        if (!isInvalid) {
                return;
        }
        for (int i = 0; i < static_cast<int>(PlaneType::Count); i++) {
                updatePlane(static_cast<PlaneType>(i));
        }
        this->isInvalid = false;
}

void CubeBatch::render(GLuint texture) {
		update();
        glBindTexture(GL_TEXTURE_2D, texture);

        for (int i = 0; i < static_cast<int>(PlaneType::Count); i++) {
                planes[i]->drawInstanced(
                    getPosVec(static_cast<PlaneType>(i)).size());
        }
}
// private

void CubeBatch::put(PlaneType type, float x, float y, float z) {
	x *= 2;
	y *= 2;
	z *= 2;
	getPosVec(type).emplace_back(x);
	getPosVec(type).emplace_back(y);
	getPosVec(type).emplace_back(z);
	this->isInvalid = true;
}

void CubeBatch::updatePlane(PlaneType type) {
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

std::vector<float>& CubeBatch::getPosVec(PlaneType type) {
        int i = static_cast<int>(type);
        assert(i >= 0 && i < static_cast<int>(PlaneType::Count));
        return posVec.at(i);
}
}  // namespace planet
