#pragma once
#ifndef GEL_SHADER_PIXELBUFFER_HPP
#define GEL_SHADER_PIXELBUFFER_HPP
#include "../gli.hpp"
#include "../util/Flag.hpp"

namespace gel {
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