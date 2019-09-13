#pragma once
#ifndef GEL_SHADER_PIXELBUFFER_HPP
#define GEL_SHADER_PIXELBUFFER_HPP
#include "../gli.hpp"
#include "../util/Flag.hpp"

namespace gel {
struct Pixel {
	GLubyte r;
	GLubyte g;
	GLubyte b;
	GLubyte a;
	Pixel();
	Pixel(GLubyte r, GLubyte g, GLubyte b, GLubyte a);
};
enum class PixelMatch {
	EqualRGB,
	EqualRGBA
};
enum class PixelReplace {
	ReplaceRGB,
	ReplaceRGBA
};
class PixelBuffer {
       public:
        explicit PixelBuffer(GLenum type);
        void init(int width, int height, int ch);
        void destroy();
        void bind();
		void bind(GLenum type);
        void unbind();
        void read() const;
        void write();
        GLubyte* map() const;
        void unmap() const;
		void transport(GLuint texture) const;
        int getWidth() const;
        int getHeight() const;
		static void replace(GLubyte* source, int width, int height, PixelMatch match, PixelReplace replace, Pixel oldPix, Pixel newPix);

       private:
        void checkRead() const;
        void checkWrite() const;
        GLuint pbo;
        Flag initFlag;
        Flag boundFlag;
        GLenum type;
        int width;
        int height;
        int ch;
};
}  // namespace gel
#endif