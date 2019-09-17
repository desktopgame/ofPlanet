#include "Sprite.hpp"
#include <glm/gtc/type_ptr.hpp>
namespace gel {
Sprite::Sprite(const std::shared_ptr<Shader>& shader, const NameRule nameRule)
    : shader(shader),
      nameRule(nameRule),
      texture(0),
      vertex(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW),
      uv(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW),
      model(),
      initFlag(false) {}

Sprite::Sprite(const std::shared_ptr<Shader>& shader)
    : Sprite(shader, NameRule()) {}

void Sprite::init(const GLuint texture, const glm::vec2 pos,
                  const glm::vec2 size, const float alpha) {
        this->texture = texture;
        this->position = pos;
        this->size = size;
        initFlag.enable();
        shader->use();
        shader->setUniform1f(nameRule.uniformAlpha, alpha);
        shader->setUniform1i(nameRule.uniformTexture, 0);
        GLuint vertexAttrib = shader->getAttribLocation(nameRule.attribVertex);
        GLuint uvAttrib = shader->getAttribLocation(nameRule.attribUV);

        vao.init();
        vertex.init();
        uv.init();

        reshape(pos, size);

        // apply
        vao.bind();
        vertex.bind();
        glVertexAttribPointer(vertexAttrib, 4, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(vertexAttrib);

        uv.bind();
        glVertexAttribPointer(uvAttrib, 2, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(uvAttrib);

        vao.unbind();
        shader->unuse();
        vertex.unbind();
        uv.unbind();
        this->model = glm::scale(model, glm::vec3(size, 1.0f));
}

void Sprite::reshape(const glm::vec2 pos, const glm::vec2 size) {
        auto& vSeq = vertex.modify();
        vSeq.clear();
        vSeq.emitVec4(pos.x + 0, pos.y + size.y, 1, 1);
        vSeq.emitVec4(pos.x + size.x, pos.y + 0, 1, 1);
        vSeq.emitVec4(pos.x + 0, pos.y + 0, 1, 1);
        vSeq.emitVec4(pos.x + 0, pos.y + size.y, 1, 1);
        vSeq.emitVec4(pos.x + size.x, pos.y + size.y, 1, 1);
        vSeq.emitVec4(pos.x + size.x, pos.y + 0, 1, 1);

        // emit
        auto& uvSeq = uv.modify();
        uvSeq.clear();
        uvSeq.emitVec2(0, 1);
        uvSeq.emitVec2(1, 0);
        uvSeq.emitVec2(0, 0);

        uvSeq.emitVec2(0, 1);
        uvSeq.emitVec2(1, 1);
        uvSeq.emitVec2(1, 0);

        // update
        vertex.bind();
        vertex.update();
        vertex.unbind();
        uv.bind();
        uv.update();
        uv.unbind();
}

void Sprite::destroy() {
        initFlag.check(true, "should be initialize");
        initFlag.disable();
        vao.destroy();
        vertex.destroy();
        uv.destroy();
}

void Sprite::draw(const std::shared_ptr<gel::Camera>& camera) {
        initFlag.check(true, "should be initialize");
        this->view = glm::inverse(camera->getInverseView());
        this->projection = camera->getOrtho();
        this->mvp = projection;
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glActiveTexture(GL_TEXTURE0);
        shader->use();
        glUniformMatrix4fv(
            shader->getUniformLocation(nameRule.uniformMVPMatrix), 1, false,
            glm::value_ptr(mvp));
        glBindTexture(GL_TEXTURE_2D, texture);
        vao.bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
        vao.unbind();
        glBindTexture(GL_TEXTURE_2D, 0);
        shader->unuse();
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
}

glm::vec2 Sprite::getPosition() const { return position; }

glm::vec2 Sprite::getSize() const { return size; }
}  // namespace gel