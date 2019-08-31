#ifndef GEL_SHADER_BUFFER_HPP
#define GEL_SHADER_BUFFER_HPP
#include <stdexcept>
#include <string>
#include "../gli.hpp"
#include "../util/Flag.hpp"
#include "Sequence.hpp"
namespace gel {
template <typename T>
class Buffer {
       public:
        explicit Buffer(GLenum target, GLenum mode);
        void bind();
        void unbind();
        void init();
        void destroy();
        void update();
        Sequence<T>& modify();
        const T* data() const;
        size_t size() const;

       private:
        GLenum target;
        GLenum mode;
        GLuint vbo;
        Sequence<T> sequence;
        Flag initFlag;
        Flag boundFlag;
        bool modified;
};

template <typename T>
inline Buffer<T>::Buffer(GLenum target, GLenum mode)
    : sequence(),
      vbo(),
      target(target),
      mode(mode),
      initFlag(false),
      boundFlag(false),
      modified(false) {}

template <typename T>
inline void Buffer<T>::bind() {
        initFlag.check(true, "should be call a initialize");
        glBindBuffer(target, vbo);
        boundFlag.enable();
}

template <typename T>
inline void Buffer<T>::unbind() {
        initFlag.check(true, "should be call a initialize");
        boundFlag.check(true, "should be call a bind()");
        glBindBuffer(target, 0);
        boundFlag.disable();
}

template <typename T>
inline void Buffer<T>::init() {
        initFlag.check(false, "already initalized");
        initFlag.enable();
        glGenBuffers(1, &vbo);
        bind();
        glBufferData(target, sizeof(T) * sequence.size(), sequence.data(),
                     mode);
        unbind();
}

template <typename T>
inline void Buffer<T>::destroy() {
        boundFlag.check(false, "should be call a unbind()");
        glDeleteBuffers(1, &vbo);
        initFlag.disable();
        sequence.clear();
}

template <typename T>
inline void Buffer<T>::update() {
        initFlag.check(true, "should be call a initialize");
        boundFlag.check(true, "should be call a bind()");
        glBufferData(target, sizeof(T) * sequence.size(), sequence.data(),
                     mode);
        this->modified = false;
}

template <typename T>
inline Sequence<T>& Buffer<T>::modify() {
        this->modified = true;
        return sequence;
}

template <typename T>
inline const T* Buffer<T>::data() const {
        return sequence.data();
}

template <typename T>
inline size_t Buffer<T>::size() const {
        return sequence.size();
}
}  // namespace gel
#endif