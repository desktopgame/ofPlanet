#include "FrameBuffer.hpp"
namespace gel {
FrameBuffer::FrameBuffer() : initFlag(false), boundFlag(false) {}

void FrameBuffer::init() {
        initFlag.check(false, "already initialized");
        initFlag.enable();
        glGenFramebuffers(1, &fbo);
}

void FrameBuffer::destroy() {
        initFlag.check(true, "should be call init()");
        glDeleteFramebuffers(1, &fbo);
}

void FrameBuffer::bind() {
        initFlag.check(true, "should be call init()");
        boundFlag.check(false, "should be call unbind()");
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        boundFlag.enable();
}

void FrameBuffer::unbind() {
        initFlag.check(true, "should be call init()");
        boundFlag.check(true, "should be call bind()");
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        boundFlag.disable();
}
}  // namespace gel