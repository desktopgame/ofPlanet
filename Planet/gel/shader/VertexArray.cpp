#include "VertexArray.hpp"
namespace gel {
VertexArray::VertexArray() : vao(), initFlag(false), boundFlag(false) {}

void VertexArray::init() {
        initFlag.check(false, "already initialized");
        initFlag.enable();
        glGenVertexArrays(1, &vao);
}

void VertexArray::destroy() {
        initFlag.check(true, "should be call init()");
        boundFlag.check(false, "should be call unbind()");
        glDeleteVertexArrays(1, &vao);
        initFlag.disable();
}

void VertexArray::bind() {
        boundFlag.check(false, "already bound");
        boundFlag.enable();
        glBindVertexArray(vao);
}

void VertexArray::unbind() {
        boundFlag.check(true, "should be call bind()");
        boundFlag.disable();
        glBindVertexArray(0);
}
}  // namespace gel