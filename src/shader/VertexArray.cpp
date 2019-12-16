#include "VertexArray.hpp"
namespace planet {

VertexArray::VertexArray() : bindFlag(false), name(0) {
        glGenVertexArrays(1, &name);
}

VertexArray::~VertexArray() { glDeleteVertexArrays(1, &name); }

void VertexArray::bind() {
        bindFlag.check(false, "already was bound");
        glBindVertexArray(name);
        bindFlag.enable();
}

void VertexArray::unbind() {
        bindFlag.check(true, "was not bound");
        glBindVertexArray(0);
        bindFlag.disable();
}
}  // namespace planet
