#include "error.hpp"
#include "../gli.hpp"
namespace gel {
void checkError(const std::string& prefix, std::ostream& ofs) {
        GLenum glE = glGetError();
        if (glE != GL_NO_ERROR) {
                const GLubyte* bytes = gluErrorString(glE);
                std::string str((const char*)bytes);
                ofs << "[" << prefix << "]" << str << "(" << glE << ")"
                    << std::endl;
        }
}

void checkError(const std::string& prefix) { checkError(prefix, std::cout); }
}  // namespace gel