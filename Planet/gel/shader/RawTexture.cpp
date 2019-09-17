#include "RawTexture.hpp"
namespace gel {
RawTexture::RawTexture(const std::shared_ptr<Shader>& shader,
                       const NameRule nameRule)
    : shader(shader),
      nameRule(nameRule),
      vertex(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW),
      uv(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW),
      vao() {}
void RawTexture::init(int width, int height) {
        initRect();
        this->width = width;
        this->height = height;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
        glBindTexture(GL_TEXTURE_2D, 0);
}
void RawTexture::destroy() { glDeleteTextures(1, &textureID); }
void RawTexture::render() {
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        shader->use();
        glBindTexture(GL_TEXTURE_2D, textureID);
        vao.bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
        vao.unbind();
        shader->unuse();
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
}
GLuint RawTexture::getTextureID() const { return textureID; }
// private
void RawTexture::initRect() {
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