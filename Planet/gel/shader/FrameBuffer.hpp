#ifndef GEL_SHADER_FRAMEBUFFER_HPP
#define GEL_SHADER_FRAMEBUFFER_HPP
#include "../gli.hpp"
#include "../util/Flag.hpp"
namespace gel {
class FrameBuffer {
       public:
        explicit FrameBuffer();
        void init();
        void destroy();
        void bind();
        void unbind();

       private:
        GLuint fbo;
        Flag initFlag;
        Flag boundFlag;
};
}  // namespace gel
#endif