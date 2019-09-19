#include "Circle.hpp"
#include <math.h>
#include "Primitive.hpp"

namespace gel {
Circle::Circle(const std::shared_ptr<Shader>& shader, const NameRule& nameRule)
    : nameRule(nameRule),
      renderer(shader),
      vertex(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW),
      lineWidth(10.0f) {}

void Circle::init(const glm::vec4 basePoint, const glm::vec4 color,
                  const float scale) {
        this->basePoint = basePoint;
        this->color = color;
		renderer.init();
        vertex.init();
        // emit
        auto& vSeq = vertex.modify();
        auto verties = Primitive::circleLine(0, scale);
        for (auto v : verties) {
                vSeq.emitVec4(v.x, v.y, v.z, 1.0f);
        }
        vertex.bind();
        vertex.update();
        vertex.unbind();
        // bind
		renderer.put(nameRule.uniformColor, RendererParameter(RendererParameterType::Uniform | RendererParameterType::FloatVec4, color));
		renderer.put(nameRule.uniformPosition, RendererParameter(RendererParameterType::Uniform | RendererParameterType::FloatVec4, basePoint));
		renderer.put(nameRule.attribVertex, RendererParameter(RendererParameterType::Attribute | RendererParameterType::FloatVec4, vertex));
		renderer.apply();
}

void Circle::destroy() {
		renderer.destroy();
        vertex.destroy();
}

void Circle::draw() {
        glLineWidth(lineWidth);
		renderer.put(nameRule.uniformColor, RendererParameter(RendererParameterType::Uniform | RendererParameterType::FloatVec4, color));
		renderer.put(nameRule.uniformPosition, RendererParameter(RendererParameterType::Uniform | RendererParameterType::FloatVec4, basePoint));
		renderer.apply(nameRule.uniformColor);
		renderer.apply(nameRule.uniformPosition);
		renderer.drawArrays(GL_LINE_LOOP, 0, 180);
}
}  // namespace gel