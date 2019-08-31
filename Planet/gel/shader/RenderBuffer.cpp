#include "RenderBuffer.hpp"
namespace gel {
RenderBuffer::RenderBuffer() : initFlag(false), boundFlag(false) {}

void RenderBuffer::init() {
        initFlag.check(false, "already initialized");
        initFlag.enable();
        glGenRenderbuffers(1, &rbo);
}

void RenderBuffer::destroy() {
        boundFlag.check(false, "should be call unbind()");
        initFlag.check(true, "should be call init()");
        initFlag.disable();
}

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
}  // namespace gel