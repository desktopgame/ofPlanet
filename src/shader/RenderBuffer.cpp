#include "RenderBuffer.hpp"
namespace planet {

RenderBuffer::RenderBuffer() : rbo(0), boundFlag(false) {
        glGenRenderbuffers(1, &rbo);
}

RenderBuffer::~RenderBuffer() { glDeleteRenderbuffers(1, &rbo); }

void RenderBuffer::bind() {
        boundFlag.check(false, "should be call unbind()");
        boundFlag.enable();
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
}

void RenderBuffer::unbind() {
        boundFlag.check(true, "should be call bind()");
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        boundFlag.disable();
}

void RenderBuffer::target(GLenum attachment) {
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER,
                                  rbo);
}
}  // namespace planet
