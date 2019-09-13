#include "PixelBuffer.hpp"
namespace gel {
PixelBuffer::PixelBuffer() : pbo(0), initFlag(false), boundFlag(false) {}
void PixelBuffer::init(int width, int height, int ch) {
        initFlag.check(false, "already initialized");
        initFlag.enable();
        this->width = width;
        this->height = height;
        this->ch = ch;
        glGenBuffers(1, &pbo);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, pbo);
        glBufferData(GL_PIXEL_UNPACK_BUFFER_ARB, width * height * ch, 0,
                     GL_DYNAMIC_DRAW);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
}
void PixelBuffer::destroy() {
        glBindBuffer(GL_ARRAY_BUFFER, pbo);
        glDeleteBuffers(1, &pbo);
        this->pbo = 0;
        initFlag.disable();
        boundFlag.disable();
}
void PixelBuffer::bind() {
        boundFlag.check(false, "already bound");
        boundFlag.enable();
        glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, pbo);
}
void PixelBuffer::unbind() {
        boundFlag.disable();
        glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, 0);
}
GLubyte *PixelBuffer::read() const {
        glReadPixels(0, 0, width, height, GL_BGRA, GL_UNSIGNED_BYTE, 0);
        GLubyte *ptr =
            (GLubyte *)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_WRITE);
        return ptr;
}
void PixelBuffer::transport(GLuint texture) const {
        boundFlag.check(true, "should be call bind");
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_BGRA,
                        GL_UNSIGNED_BYTE, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
}
int PixelBuffer::getWidth() const { return width; }
int PixelBuffer::getHeight() const { return height; }
}  // namespace gel