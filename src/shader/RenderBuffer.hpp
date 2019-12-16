#pragma once
#ifndef SHADER_RENDERBUFFER_HPP
#define SHADER_RENDERBUFFER_HPP
#include <ofTypes.h>

#include "Flag.hpp"
namespace planet {

class RenderBuffer {
       public:
        explicit RenderBuffer();
        ~RenderBuffer();
        void bind();
        void unbind();
        void target(GLenum attachment);

       private:
        GLuint rbo;
        Flag boundFlag;
};
}  // namespace planet
#endif