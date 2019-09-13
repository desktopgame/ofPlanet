#include "PixelBuffer.hpp"
namespace gel {
PixelBuffer::PixelBuffer(GLenum type)
    : pbo(0), initFlag(false), boundFlag(false), type(type) {}
void PixelBuffer::init(int width, int height, int ch) {
        initFlag.check(false, "already initialized");
        initFlag.enable();
        this->width = width;
        this->height = height;
        this->ch = ch;
        glGenBuffers(1, &pbo);
        glBindBuffer(type, pbo);
        glBufferData(type, (width * height * ch), 0, GL_DYNAMIC_DRAW);
        glBindBuffer(type, 0);
}
void PixelBuffer::destroy() {
        glDeleteBuffers(1, &pbo);
        this->pbo = 0;
        initFlag.disable();
        boundFlag.disable();
}
void PixelBuffer::bind() {
        boundFlag.check(false, "already bound");
        boundFlag.enable();
        glBindBuffer(type, pbo);
}
void PixelBuffer::bind(GLenum type)
{
	this->type = type;
	bind();
}
void PixelBuffer::unbind() {
        boundFlag.disable();
        glBindBuffer(type, 0);
}
void PixelBuffer::read() const {
        boundFlag.check(true, "should be call bind");
        checkRead();
        glReadPixels(0, 0, width, height, GL_BGRA, GL_UNSIGNED_BYTE, 0);
}
void PixelBuffer::write() {
        boundFlag.check(true, "should be call bind");
        checkWrite();
        glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, 0);
}
GLubyte *PixelBuffer::map() const {
        boundFlag.check(true, "should be call bind");
        checkRead();
        GLubyte *ptr = (GLubyte *)glMapBuffer(type, GL_READ_WRITE);
        return ptr;
}
void PixelBuffer::unmap() const {
        boundFlag.check(true, "should be call bind");
        checkRead();
        glUnmapBufferARB(type);
}
void PixelBuffer::transport(GLuint texture) const
{
	boundFlag.check(true, "should be call bind");
	checkWrite();
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_BGRA, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
int PixelBuffer::getWidth() const { return width; }
int PixelBuffer::getHeight() const { return height; }
void PixelBuffer::replace(GLubyte* source, int width, int height, PixelMatch match, PixelReplace replace, Pixel oldPix, Pixel newPix)
{
	for (int i = 0; i < width*height; ++i) {
		GLubyte r = source[4 * i + 0];
		GLubyte g = source[4 * i + 1];
		GLubyte b = source[4 * i + 2];
		GLubyte a = source[4 * i + 3];
		bool matchR = r == (oldPix.r);
		bool matchG = g == (oldPix.g);
		bool matchB = b == (oldPix.b);
		bool matchA = a == (oldPix.a);
		bool replaceColor = false;
		if (match == PixelMatch::EqualRGB && matchR && matchG && matchB) {
			replaceColor = true;
		} else if (match == PixelMatch::EqualRGBA && matchR && matchG && matchB && matchA) {
			replaceColor = true;
		}
		if (replaceColor) {
			if (replace == PixelReplace::ReplaceRGB || replace == PixelReplace::ReplaceRGBA) {
				source[4 * i + 0] = newPix.r;
				source[4 * i + 1] = newPix.g;
				source[4 * i + 2] = newPix.b;
			}
			if (replace == PixelReplace::ReplaceRGBA) {
				source[4 * i + 3] = newPix.a;
			}
		}
	}
}
void PixelBuffer::checkRead() const {
        assert(type == GL_PIXEL_PACK_BUFFER_ARB);
}
void PixelBuffer::checkWrite() const {
        assert(type == GL_PIXEL_UNPACK_BUFFER_ARB);
}
// Pixel
Pixel::Pixel() : r(0), g(0), b(0), a(1) {
}
Pixel::Pixel(GLubyte r, GLubyte g, GLubyte b, GLubyte a)
	:r(r),g(g),b(b),a(a)
{
}
}  // namespace gel