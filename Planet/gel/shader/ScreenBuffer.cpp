#include "ScreenBuffer.hpp"
namespace gel {
ScreenBuffer::ScreenBuffer(const std::shared_ptr<Shader>& shader, const NameRule nameRule, int width,
                           int height)
    : width(width),
      height(height),
      frameBuffer(),
      renderBuffer(),
      shader(shader),
      nameRule(nameRule),
      vertex(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW),
      uv(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW),
      vao() {}

void ScreenBuffer::init() {
        initRect();
        // init frame buffer
        frameBuffer.init();
        frameBuffer.bind();

        // gen texture
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D, texture, 0);

        // init render buffer
        renderBuffer.init();
        renderBuffer.bind();
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width,
                              height);
        renderBuffer.target(GL_DEPTH_STENCIL_ATTACHMENT);
        // renderBuffer.unbind();

        frameBuffer.unbind();
        renderBuffer.unbind();
        glBindTexture(GL_TEXTURE_2D, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
            GL_FRAMEBUFFER_COMPLETE) {
                throw std::logic_error("invalid state");
        }
}

void ScreenBuffer::destroy() {
        vao.destroy();
        vertex.destroy();
        uv.destroy();
        frameBuffer.destroy();
        renderBuffer.destroy();
}

void ScreenBuffer::bind() {
        frameBuffer.bind();
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.3f, 0.3f, 1.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ScreenBuffer::unbind() { frameBuffer.unbind(); }

void ScreenBuffer::render() {
        glDisable(GL_DEPTH_TEST);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader->use();
        glBindTexture(GL_TEXTURE_2D, texture);
        vao.bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
        vao.unbind();
        shader->unuse();
}
// private
void ScreenBuffer::initRect() {
        vao.init();
        vertex.init();
        uv.init();

        auto& vSeq = vertex.modify();
        vSeq.emitVec4(-1, 1, 0, 1);
        vSeq.emitVec4(-1, -1, 0, 1);
        vSeq.emitVec4(1, -1, 0, 1);
        vSeq.emitVec4(-1, 1, 0, 1);
        vSeq.emitVec4(1, -1, 0, 1);
        vSeq.emitVec4(1, 1, 0, 1);

        auto& uvSeq = uv.modify();
        uvSeq.emitVec2(0, 1);
        uvSeq.emitVec2(0, 0);
        uvSeq.emitVec2(1, 0);
        uvSeq.emitVec2(0, 1);
        uvSeq.emitVec2(1, 0);
        uvSeq.emitVec2(1, 1);

        vertex.bind();
        vertex.update();
        vertex.unbind();

        uv.bind();
        uv.update();
        uv.unbind();
        // setup vao
        shader->use();
        shader->setUniform1i("uTexture", 0);
        vao.bind();

        vertex.bind();
        shader->setVertexAttribPointer(nameRule.attribVertex, 4, GL_FLOAT,
                                      GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(
            shader->getAttribLocation(nameRule.attribVertex));

        uv.bind();
        shader->setVertexAttribPointer(nameRule.attribUV, 2, GL_FLOAT, GL_FALSE,
                                      0, NULL);
        glEnableVertexAttribArray(shader->getAttribLocation(nameRule.attribUV));

        vao.unbind();
        shader->unuse();
        vertex.unbind();
        uv.unbind();
}
}  // namespace gel