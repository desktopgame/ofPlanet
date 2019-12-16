#include "FrameBuffer.hpp"
namespace planet {

FrameBuffer::FrameBuffer() : fbo(0), boundFlag(false) {
        glGenFramebuffers(1, &fbo);
}

FrameBuffer::~FrameBuffer() { glDeleteFramebuffers(1, &fbo); }

void FrameBuffer::bind() {
        boundFlag.check(false, "should be call unbind()");
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        boundFlag.enable();
}

void FrameBuffer::unbind() {
        boundFlag.check(true, "should be call bind()");
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        boundFlag.disable();
}
}  // namespace planet
