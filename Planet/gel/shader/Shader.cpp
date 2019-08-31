#include "Shader.hpp"
#include <iostream>
#include <stdexcept>
namespace gel {
Shader::Shader() : linkFlag(false), useFlag(false) {}

Shader Shader::make(const std::string& vertShader,
                    const std::string& fragShader) {
        Shader s;
        s.link(vertShader, fragShader);
        return s;
}

void Shader::link(const std::string& vertShader,
                  const std::string& fragShader) {
        linkFlag.check(false, "already linked");
        linkFlag.enable();
        // create vertex shader
        GLuint vertShId = glCreateShader(GL_VERTEX_SHADER);
        if (vertShId == 0) {
                checkShaderError(vertShId, "failed a create vertex shader");
        }
        const char* vertShCStr = vertShader.c_str();
        glShaderSource(vertShId, 1, &vertShCStr, NULL);
        glCompileShader(vertShId);
        // check error
        GLint compileErr;
        glGetShaderiv(vertShId, GL_COMPILE_STATUS, &compileErr);
        if (GL_FALSE == compileErr) {
                checkShaderError(vertShId, "failed a compile vertex shader");
        }
        // create fragment shader
        GLuint fragShId = glCreateShader(GL_FRAGMENT_SHADER);
        if (fragShId == 0) {
                checkShaderError(fragShId, "failed a create fragment shader");
        }
        const char* fragShCStr = fragShader.c_str();
        glShaderSource(fragShId, 1, &fragShCStr, NULL);
        glCompileShader(fragShId);
        // check error
        glGetShaderiv(fragShId, GL_COMPILE_STATUS, &compileErr);
        if (GL_FALSE == compileErr) {
                checkShaderError(fragShId, "failed a compile fragment shader");
        }
        // create program
        this->program = glCreateProgram();
        if (this->program == 0) {
                throw std::logic_error("failed a create program");
        }
        glAttachShader(program, vertShId);
        glAttachShader(program, fragShId);
        glLinkProgram(program);
        GLint status;
        glGetProgramiv(program, GL_LINK_STATUS, &status);
        if (GL_FALSE == status) {
                throw std::logic_error("failed a link program");
        }
        glDeleteShader(vertShId);
        glDeleteShader(fragShId);
}

void Shader::unlink() {
        linkFlag.check(true, "should be call link()");
        linkFlag.disable();
        glDeleteProgram(program);
}

void Shader::use() {
        linkFlag.check(true, "should be call link()");
        useFlag.check(false, "should be call unuse()");
        useFlag.enable();
        glUseProgram(program);
}

void Shader::unuse() {
        linkFlag.check(true, "should be call link()");
        useFlag.check(true, "should be call use()");
        glUseProgram(0);
        useFlag.disable();
}

void Shader::setUniform1f(const std::string& uniform, float a) const {
        glUniform1f(getUniformLocation(uniform), a);
}

void Shader::setUniform1fv(const std::string& uniform, int count,
                           float* v) const {
        glUniform1fv(getUniformLocation(uniform), count, v);
}

void Shader::setUniform1i(const std::string& uniform, int a) const {
        glUniform1i(getUniformLocation(uniform), a);
}

void Shader::setUniform1iv(const std::string& uniform, int count,
                           int* v) const {
        glUniform1iv(getUniformLocation(uniform), count, v);
}

void Shader::setUniform2f(const std::string& uniform, float a, float b) const {
        glUniform2f(getUniformLocation(uniform), a, b);
}

void Shader::setUniform2fv(const std::string& uniform, int count,
                           float* v) const {
        glUniform2fv(getUniformLocation(uniform), count, v);
}

void Shader::setUniform3f(const std::string& uniform, float a, float b,
                          float c) const {
        glUniform3f(getUniformLocation(uniform), a, b, c);
}

void Shader::setUniform3fv(const std::string& uniform, int count,
                           float* v) const {
        glUniform3fv(getUniformLocation(uniform), count, v);
}

void Shader::setUniform4f(const std::string& uniform, float a, float b, float c,
                          float d) const {
        glUniform4f(getUniformLocation(uniform), a, b, c, d);
}

void Shader::setUniform4fv(const std::string& uniform, int count,
                           float* v) const {
        glUniform4fv(getUniformLocation(uniform), count, v);
}

void Shader::setUniformMatrix2fv(const std::string& uniform, int count,
                                 GLboolean transpose, const float* v) const {
        glUniformMatrix2fv(getUniformLocation(uniform), count, transpose, v);
}

void Shader::setUniformMatrix3fv(const std::string& uniform, int count,
                                 GLboolean transpose, const float* v) const {
        glUniformMatrix3fv(getUniformLocation(uniform), count, transpose, v);
}

void Shader::setUniformMatrix4fv(const std::string& uniform, int count,
                                 GLboolean transpose, const float* v) const {
        glUniformMatrix4fv(getUniformLocation(uniform), count, transpose, v);
}

void Shader::setVertexAttribPointer(const std::string& attrib, int size,
                                    GLenum type, GLboolean normalized,
                                    int stride, const GLvoid* pointer) const {
        glVertexAttribPointer(getAttribLocation(attrib), size, type, normalized,
                              stride, pointer);
}

GLint Shader::getAttribLocation(const std::string& attrib) const {
        linkFlag.check(true, "should be call link()");
        return glGetAttribLocation(program, attrib.c_str());
}

GLint Shader::getUniformLocation(const std::string& uniform) const {
        linkFlag.check(true, "should be call link()");
        return glGetUniformLocation(program, uniform.c_str());
}

GLuint Shader::getProgram() const { return program; }
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
}  // namespace gel