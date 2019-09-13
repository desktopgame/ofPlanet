#ifndef GEL_SHADER_SCREENBUFFER_HPP
#define GEL_SHADER_SCREENBUFFER_HPP
#include "../gli.hpp"
#include "../util/Flag.hpp"
#include "Buffer.hpp"
#include "FrameBuffer.hpp"
#include "NameRule.hpp"
#include "RenderBuffer.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"

namespace gel {
class ScreenBuffer {
       public:
        explicit ScreenBuffer(const std::shared_ptr<Shader>& shader,
                              const NameRule nameRule);
        void init(int width, int height);
        void destroy();
        void bind();
        void unbind();
        void render();
		GLuint texture;

       private:
        void initRect();
        int width;
        int height;
        FrameBuffer frameBuffer;
        RenderBuffer renderBuffer;

        std::shared_ptr<Shader> shader;
        NameRule nameRule;
        VertexArray vao;
        Buffer<GLfloat> vertex;
        Buffer<GLfloat> uv;
};
}  // namespace gel
#endif