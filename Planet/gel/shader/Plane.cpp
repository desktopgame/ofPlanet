#include "Plane.hpp"
namespace gel {
Plane::Plane(Shader& shader, const NameRule& nameRule)
    : shader(shader),
      type(PlaneType::Forward),
      fwdIndexBuf(GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW),
      backIndexBuf(GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW),
      leftIndexBuf(GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW),
      rightIndexBuf(GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW),
      topIndexBuf(GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW),
      bottomIndexBuf(GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW),
      vertexBuf(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW),
      uvBuf(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW),
      normalBuf(GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW),
      nameRule(nameRule),
      initFlag(false) {}

Plane::Plane(Shader& shader) : Plane(shader, NameRule()) {}

void Plane::init(float size) {
        initFlag.check(false, "already initalized");
        this->size = size;
        initFlag.enable();
        shader.use();
        vao.init();
        initBuffers();
        //
        // init vao
        //
        this->vertexAttrib = shader.getAttribLocation(nameRule.attribVertex);
        vao.bind();
        vertexBuf.bind();
        glVertexAttribPointer(vertexAttrib, 4, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(vertexAttrib);
        this->normalAttrib = shader.getAttribLocation(nameRule.attribNormal);
        normalBuf.bind();
        glVertexAttribPointer(normalAttrib, 4, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(normalAttrib);
        this->uvAttrib = shader.getAttribLocation(nameRule.attribUV);
        uvBuf.bind();
        glVertexAttribPointer(uvAttrib, 2, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(uvAttrib);
        vao.unbind();
        vertexBuf.unbind();
        normalBuf.unbind();
        uvBuf.unbind();
        shader.unuse();
}

void Plane::destroy() {
        vao.destroy();
        fwdIndexBuf.destroy();
        backIndexBuf.destroy();
        leftIndexBuf.destroy();
        rightIndexBuf.destroy();
        topIndexBuf.destroy();
        bottomIndexBuf.destroy();
        vertexBuf.destroy();
        uvBuf.destroy();
        normalBuf.destroy();
        initFlag.disable();
}

void Plane::draw() {
        shader.use();
        auto& src = getIndexBufferFromType(type);
        vao.bind();
        src.bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
        src.unbind();
        vao.unbind();
        shader.unuse();
}

void Plane::render(Buffer<float> posBuf, int count) {
        auto& src = getIndexBufferFromType(type);
        bind();
        src.bind();
        shader.use();
        GLuint posAttrib = shader.getAttribLocation(nameRule.attribPosition);

        posBuf.bind();
        glVertexAttribPointer(posAttrib, 4, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(posAttrib);

        glVertexAttribDivisor(shader.getAttribLocation(nameRule.attribVertex),
                              0);
        glVertexAttribDivisor(shader.getAttribLocation(nameRule.attribUV), 0);
        glVertexAttribDivisor(shader.getAttribLocation(nameRule.attribNormal),
                              0);
        glVertexAttribDivisor(shader.getAttribLocation(nameRule.attribPosition),
                              1);

        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL,
                                count);
        posBuf.unbind();

        shader.unuse();
        src.unbind();
        unbind();
}

void Plane::bind() { vao.bind(); }

void Plane::unbind() { vao.unbind(); }

void Plane::setType(const PlaneType type) {
        if (this->type == type) {
                return;
        }
        this->type = type;
}
PlaneType Plane::getType() const { return type; }
// private
void Plane::initBuffers() {
        fwdIndexBuf.init();
        backIndexBuf.init();
        leftIndexBuf.init();
        rightIndexBuf.init();
        topIndexBuf.init();
        bottomIndexBuf.init();
        vertexBuf.init();
        uvBuf.init();
        normalBuf.init();
        auto& fwdSeq = fwdIndexBuf.modify();
        auto& backSeq = backIndexBuf.modify();
        auto& leftSeq = leftIndexBuf.modify();
        auto& rightSeq = rightIndexBuf.modify();
        auto& topSeq = topIndexBuf.modify();
        auto& bottomSeq = bottomIndexBuf.modify();
        auto& vSeq = vertexBuf.modify();
        auto& uvSeq = uvBuf.modify();
        auto& normalSeq = normalBuf.modify();
        fwdSeq.emitVec3(0, 1, 2);
        fwdSeq.emitVec3(2, 3, 0);
        topSeq.emitVec3(4, 5, 6);
        topSeq.emitVec3(6, 7, 4);
        backSeq.emitVec3(8, 9, 10);
        backSeq.emitVec3(10, 11, 8);
        bottomSeq.emitVec3(12, 13, 14);
        bottomSeq.emitVec3(14, 15, 12);
        leftSeq.emitVec3(16, 17, 18);
        leftSeq.emitVec3(18, 19, 16);
        rightSeq.emitVec3(20, 21, 22);
        rightSeq.emitVec3(22, 23, 20);
        fwdIndexBuf.bind();
        fwdIndexBuf.update();
        fwdIndexBuf.unbind();
        backIndexBuf.bind();
        backIndexBuf.update();
        backIndexBuf.unbind();
        leftIndexBuf.bind();
        leftIndexBuf.update();
        leftIndexBuf.unbind();
        rightIndexBuf.bind();
        rightIndexBuf.update();
        rightIndexBuf.unbind();
        topIndexBuf.bind();
        topIndexBuf.update();
        topIndexBuf.unbind();
        bottomIndexBuf.bind();
        bottomIndexBuf.update();
        bottomIndexBuf.unbind();
        // front
        vSeq.emitVec4(-size, -size, size, size);
        vSeq.emitVec4(size, -size, size, size);
        vSeq.emitVec4(size, size, size, size);
        vSeq.emitVec4(-size, size, size, size);
        addNormals(normalBuf, 4, 0, 0, 1, 0);
        uvSeq.emitVec2(0, 1);
        uvSeq.emitVec2(1, 1);
        uvSeq.emitVec2(1, 0);
        uvSeq.emitVec2(0, 0);
        // top
        vSeq.emitVec4(-size, size, size, size);
        vSeq.emitVec4(size, size, size, size);
        vSeq.emitVec4(size, size, -size, size);
        vSeq.emitVec4(-size, size, -size, size);
        addNormals(normalBuf, 4, 0, 1, 0, 0);
        uvSeq.emitVec2(0, 0);
        uvSeq.emitVec2(1, 0);
        uvSeq.emitVec2(1, 1);
        uvSeq.emitVec2(0, 1);
        // back
        vSeq.emitVec4(size, -size, -size, size);
        vSeq.emitVec4(-size, -size, -size, size);
        vSeq.emitVec4(-size, size, -size, size);
        vSeq.emitVec4(size, size, -size, size);
        addNormals(normalBuf, 4, 0, 0, -1, 0);
        uvSeq.emitVec2(0, 1);
        uvSeq.emitVec2(1, 1);
        uvSeq.emitVec2(1, 0);
        uvSeq.emitVec2(0, 0);
        // bottom
        vSeq.emitVec4(-size, -size, -size, size);
        vSeq.emitVec4(size, -size, -size, size);
        vSeq.emitVec4(size, -size, size, size);
        vSeq.emitVec4(-size, -size, size, size);
        addNormals(normalBuf, 4, 0, -1, 0, 0);
        uvSeq.emitVec2(0, 0);
        uvSeq.emitVec2(1, 0);
        uvSeq.emitVec2(1, 1);
        uvSeq.emitVec2(0, 1);
        // left
        vSeq.emitVec4(-size, -size, -size, size);
        vSeq.emitVec4(-size, -size, size, size);
        vSeq.emitVec4(-size, size, size, size);
        vSeq.emitVec4(-size, size, -size, size);
        addNormals(normalBuf, 4, -1, 0, 0, 0);
        uvSeq.emitVec2(0, 1);
        uvSeq.emitVec2(1, 1);
        uvSeq.emitVec2(1, 0);
        uvSeq.emitVec2(0, 0);
        // right
        vSeq.emitVec4(size, -size, size, size);
        vSeq.emitVec4(size, -size, -size, size);
        vSeq.emitVec4(size, size, -size, size);
        vSeq.emitVec4(size, size, size, size);
        addNormals(normalBuf, 4, 1, 0, 0, 0);
        uvSeq.emitVec2(0, 1);
        uvSeq.emitVec2(1, 1);
        uvSeq.emitVec2(1, 0);
        uvSeq.emitVec2(0, 0);
        vertexBuf.bind();
        vertexBuf.update();
        vertexBuf.unbind();
        uvBuf.bind();
        uvBuf.update();
        uvBuf.unbind();
        normalBuf.bind();
        normalBuf.update();
        normalBuf.unbind();
        setType(PlaneType::Forward);
}

void Plane::addNormals(Buffer<GLfloat>& buf, int count, float x, float y,
                       float z, float w) {
        auto& seq = buf.modify();
        for (int i = 0; i < count; i++) {
                seq.emitVec4(x, y, z, w);
        }
}

Buffer<GLushort>& Plane::getIndexBufferFromType(const PlaneType type) {
        switch (type) {
                case PlaneType::Forward:
                        return fwdIndexBuf;
                case PlaneType::Backward:
                        return backIndexBuf;
                case PlaneType::Left:
                        return leftIndexBuf;
                case PlaneType::Right:
                        return rightIndexBuf;
                case PlaneType::Top:
                        return topIndexBuf;
                case PlaneType::Bottom:
                        return bottomIndexBuf;
                default:
                        break;
        }
        throw std::logic_error("invalid operation");
}
}  // namespace gel