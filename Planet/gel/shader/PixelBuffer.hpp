#pragma once
#ifndef GEL_SHADER_PIXELBUFFER_HPP
#define GEL_SHADER_PIXELBUFFER_HPP
#include "../gli.hpp"
#include "../util/Flag.hpp"

namespace gel {
class PixelBuffer {
       public:
        explicit PixelBuffer();
        void init(int width, int height, int ch);
        void destroy();
        void bind();
        void unbind();
        GLubyte* read() const;
        void transport(GLuint texture) const;
        int getWidth() const;
        int getHeight() const;

       private:
        GLuint pbo;
        Flag initFlag;
        Flag boundFlag;
        int width;
        int height;
        int ch;
};
}  // namespace gel
#endif