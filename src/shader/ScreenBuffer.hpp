#pragma once
#ifndef SHADER_SCREENBUFFER_HPP
#define SHADER_SCREENBUFFER_HPP
#include <ofVbo.h>
#include <ofVec4f.h>

#include "FrameBuffer.hpp"
#include "NameSet.hpp"
#include "RenderBuffer.hpp"
namespace planet {

class Shader;
class ScreenBuffer {
       public:
        explicit ScreenBuffer(const NameSet& nameSet, int width, int height);
        ~ScreenBuffer();
        void bind();
        void unbind();
        void render();

       private:
        void initRect();
        NameSet nameSet;
        int width;
        int height;
        GLuint texture;
        FrameBuffer frameBuffer;
        RenderBuffer renderBuffer;

        std::shared_ptr<Shader> shader;
        glm::vec4 clearColor;
        ofVbo vao;
};
}  // namespace planet
#endif