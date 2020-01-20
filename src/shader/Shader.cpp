#include "Shader.hpp"

#include <ofMatrix3x3.h>
#include <ofMatrix4x4.h>

#include <iostream>
#include <stdexcept>
namespace planet {
Shader::Shader() : linkFlag(false), useFlag(false), shader() {}

Shader::~Shader() {}

void Shader::load(const std::string& vertShader,
                  const std::string& fragShader) {
        bool loaded = shader.load(vertShader, fragShader);
        assert(loaded);
}

void Shader::use() { shader.begin(); }

void Shader::unuse() { shader.end(); }

void Shader::setUniform1f(const std::string& uniform, float a) const {
        shader.setUniform1f(uniform, a);
}

void Shader::setUniform1fv(const std::string& uniform, int count,
                           float* v) const {
        shader.setUniform1fv(uniform, v, count);
}

void Shader::setUniform1i(const std::string& uniform, int a) const {
        shader.setUniform1i(uniform, a);
}

void Shader::setUniform1iv(const std::string& uniform, int count,
                           int* v) const {
        shader.setUniform1iv(uniform, v, count);
}

void Shader::setUniform2f(const std::string& uniform, float a, float b) const {
        shader.setUniform2f(uniform, a, b);
}

void Shader::setUniform2fv(const std::string& uniform, int count,
                           float* v) const {
        shader.setUniform2fv(uniform, v, count);
}

void Shader::setUniform2i(const std::string& uniform, int a, int b) const {
        shader.setUniform2i(uniform, a, b);
}

void Shader::setUniform2iv(const std::string& uniform, int count,
                           int* v) const {
        shader.setUniform2iv(uniform, v, count);
}

void Shader::setUniform3f(const std::string& uniform, float a, float b,
                          float c) const {
        shader.setUniform3f(uniform, a, b, c);
}

void Shader::setUniform3fv(const std::string& uniform, int count,
                           float* v) const {
        shader.setUniform3fv(uniform, v, count);
}

void Shader::setUniform3i(const std::string& uniform, int a, int b,
                          int c) const {
        shader.setUniform3i(uniform, a, b, c);
}

void Shader::setUniform3iv(const std::string& uniform, int count,
                           int* v) const {
        shader.setUniform3iv(uniform, v, count);
}

void Shader::setUniform4f(const std::string& uniform, float a, float b, float c,
                          float d) const {
        shader.setUniform4f(uniform, a, b, c, d);
}

void Shader::setUniform4fv(const std::string& uniform, int count,
                           float* v) const {
        shader.setUniform4fv(uniform, v, count);
}

void Shader::setUniform4i(const std::string& uniform, int a, int b, int c,
                          int d) const {
        shader.setUniform4i(uniform, a, b, c, d);
}

void Shader::setUniform4iv(const std::string& uniform, int count,
                           int* v) const {
        shader.setUniform4iv(uniform, v, count);
}

void Shader::setUniformMatrix2fv(const std::string& uniform, int count,
                                 GLboolean transpose, const float* v) const {
        throw std::logic_error("not supported");
}

void Shader::setUniformMatrix3fv(const std::string& uniform, int count,
                                 GLboolean transpose, const float* v) const {
        throw std::logic_error("not supported");
}

void Shader::setUniformMatrix4fv(const std::string& uniform, int count,
                                 GLboolean transpose, const float* v) const {
        throw std::logic_error("not supported");
}

void Shader::setUniformMatrix2f(const std::string& uniform, int count,
                                GLboolean transpose, const glm::mat2& m) const {
        throw std::logic_error("not supported");
}

void Shader::setUniformMatrix3f(const std::string& uniform, int count,
                                GLboolean transpose, const glm::mat3& m) const {
        shader.setUniformMatrix3f(uniform, m, count);
}

void Shader::setUniformMatrix4f(const std::string& uniform, int count,
                                GLboolean transpose, const glm::mat4& m) const {
        shader.setUniformMatrix4f(uniform, m, count);
}

void Shader::setVertexAttribPointer(const std::string& attrib, int size,
                                    GLenum type, GLboolean normalized,
                                    int stride, const GLvoid* pointer) const {
        throw std::logic_error("not supported");
}

void Shader::enableVertexAttribArray(const std::string& attrib) const {
        throw std::logic_error("not supported");
}

void Shader::setVertexAttribDivisor(const std::string& attrib, GLuint divisor) {
        throw std::logic_error("not supported");
}

GLint Shader::getAttribLocation(const std::string& attrib) const {
        return shader.getAttributeLocation(attrib);
}

GLint Shader::getUniformLocation(const std::string& uniform) const {
        return shader.getUniformLocation(uniform);
}

GLuint Shader::getProgram() const { return shader.getProgram(); }
// private
void Shader::checkShaderError(GLuint shader, const std::string& baseStr) {
        GLsizei logSize;
        GLsizei bufSize = 2048;
        char buf[2048] = {0};
        glGetShaderInfoLog(shader, bufSize, &logSize, buf);
        std::string str = baseStr + '\n' + buf;
        std::cerr << str << std::endl;
        throw std::logic_error(str);
}
void Shader::checkProgramError(GLuint shader, const std::string& baseStr) {
        GLsizei logSize;
        GLsizei bufSize = 2048;
        char buf[2048] = {0};
        glGetProgramInfoLog(shader, bufSize, &logSize, buf);
        std::string str = baseStr + '\n' + buf;
        std::cerr << str << std::endl;
        throw std::logic_error(str);
}
}  // namespace planet
