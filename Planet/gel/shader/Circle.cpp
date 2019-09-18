#include "Circle.hpp"
#include <math.h>
#include "Primitive.hpp"

namespace gel {
Circle::Circle(const std::shared_ptr<Shader>& shader, const NameRule& nameRule)
    : shader(shader),
      nameRule(nameRule),
      vao(),
      vertex(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW),
      lineWidth(10.0f) {}

void Circle::init(const glm::vec4 basePoint, const glm::vec4 color,
                  const float scale) {
        this->basePoint = basePoint;
        this->color = color;
        vao.init();
        vertex.init();
        // emit
        auto& vSeq = vertex.modify();
		auto verties = Primitive::circle(0, scale);
		for (auto v : verties) {
			vSeq.emitVec4(v.x, v.y, v.z, 1.0f);
		}
        vertex.bind();
        vertex.update();
        vertex.unbind();
        // bind
        shader->use();
        vao.bind();
        GLuint vertexAttrib = shader->getAttribLocation(nameRule.attribVertex);
        vertex.bind();
        shader->setUniform4f(nameRule.uniformColor, color.x, color.y, color.z,
                             color.w);
        shader->setUniform4f(nameRule.uniformPosition, basePoint.x, basePoint.y,
                             basePoint.z, basePoint.w);
        glVertexAttribPointer(vertexAttrib, 4, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(vertexAttrib);
        vao.unbind();
        shader->unuse();
        vertex.unbind();
}

void Circle::destroy() {
        vao.destroy();
        vertex.destroy();
}

void Circle::draw() {
        glLineWidth(lineWidth);
        shader->use();
        shader->setUniform4f(nameRule.uniformColor, color.x, color.y, color.z,
                             color.w);
        shader->setUniform4f(nameRule.uniformPosition, basePoint.x, basePoint.y,
                             basePoint.z, basePoint.w);
        vao.bind();
        glDrawArrays(GL_LINE_LOOP, 0, 180);
        vao.unbind();
        shader->unuse();
}
}  // namespace gel