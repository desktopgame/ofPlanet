#ifndef GEL_UTIL_NONCOPYABLE_HPP
#define GEL_UTIL_NONCOPYABLE_HPP

#define NON_COPYABLE_PRIVATE(type)   \
       private:                      \
        type(const type &) = delete; \
        type &operator=(const type &) = delete;

namespace gel {
template <class T>
class NonCopyable {
       protected:
        NonCopyable() {}
        ~NonCopyable() {}

       private:
        NonCopyable(const NonCopyable &) = delete;
        T &operator=(const T &) = delete;
};
}  // namespace gel
#endif