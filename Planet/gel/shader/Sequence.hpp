#ifndef GEL_SHADER_SEQUENCE_HPP
#define GEL_SHADER_SEQUENCE_HPP
#include <vector>
namespace gel {
template <typename T>
class Sequence {
       public:
        explicit Sequence();
        void emit(const T a);
        void emitVec2(const T x, const T y);
        void emitVec3(const T x, const T y, const T z);
        void emitVec4(const T x, const T y, const T z, const T w);
        void clear();
        size_t size() const;
        const T at(int i) const;
        const T* data() const;

       private:
        std::vector<T> vec;
};

template <typename T>
Sequence<T>::Sequence() : vec() {}

template <typename T>
void Sequence<T>::emit(const T a) {
        vec.push_back(a);
}

template <typename T>
void Sequence<T>::emitVec2(const T x, const T y) {
        emit(x);
        emit(y);
}

template <typename T>
void Sequence<T>::emitVec3(const T x, const T y, const T z) {
        emitVec2(x, y);
        emit(z);
}

template <typename T>
void Sequence<T>::emitVec4(const T x, const T y, const T z, const T w) {
        emitVec3(x, y, z);
        emit(w);
}

template <typename T>
void Sequence<T>::clear() {
        vec.clear();
}

template <typename T>
size_t Sequence<T>::size() const {
        return vec.size();
}

template <typename T>
const T Sequence<T>::at(int i) const {
        return vec.at(i);
}

template <typename T>
const T* Sequence<T>::data() const {
        return vec.data();
}
}  // namespace gel
#endif