#ifndef GEL_SHADER_RENDERBUFFER_HPP
#define GEL_SHADER_RENDERBUFFER_HPP
#include "../gli.hpp"
#include "../util/Flag.hpp"
namespace gel {
class RenderBuffer {
       public:
        explicit RenderBuffer();
        void init();
        void destroy();
        void bind();
        void unbind();
        void target(GLenum attachment);

       private:
        GLuint rbo;
        Flag initFlag;
        Flag boundFlag;
};
}  // namespace gel
#endif