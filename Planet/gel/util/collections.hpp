#ifndef GEL_UTIL_COLLECTIONS_HPP
#define GEL_UTIL_COLLECTIONS_HPP
#include <stdexcept>
#include <vector>
namespace gel {
namespace collections {

template <typename T>
inline void fill(std::vector<T>& a, int size, const T value) {
        for (int i = a.size(); i < size; i++) {
                a.push_back(value);
        }
}

template <typename T>
void limit(const std::vector<T>& a, int size) {
        if (a.size() > size) {
                throw std::logic_error("was over of container limit");
        }
}
}  // namespace collections
}  // namespace gel
#endif