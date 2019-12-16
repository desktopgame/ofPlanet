#pragma once
#ifndef SHADER_FRAMEBUFFER_HPP
#define SHADER_FRAMEBUFFER_HPP
#include <ofTypes.h>

#include "Flag.hpp"
namespace planet {

class FrameBuffer {
       public:
        explicit FrameBuffer();
        ~FrameBuffer();
        void bind();
        void unbind();

       private:
        GLuint fbo;
        Flag boundFlag;
};
}  // namespace planet
#endif