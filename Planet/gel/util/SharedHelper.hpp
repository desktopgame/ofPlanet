#pragma once
#ifndef GEL_UTIL_SHAREDHELPER_HPP
#define GEL_UTIL_SHAREDHELPER_HPP
// http://wkm-ghn.hateblo.jp/entry/2016/05/20/225905
namespace gel {
template <typename T>
class SharedHelper {
        // すべてプライベートメンバ

        friend T;

        struct entity : public T {
                template <typename... Args>
                entity(Args&&... args) : T(std::forward<Args>(args)...) {}
        };

        template <typename... Args>
        static std::shared_ptr<T> make_shared(Args&&... args) {
                return std::make_shared<entity>(std::forward<Args>(args)...);
        }

        template <typename Alloc, typename... Args>
        static std::shared_ptr<T> allocate_shared(const Alloc& alloc,
                                                  Args&&... args) {
                return std::allocate_shared<entity>(
                    alloc, std::forward<Args>(args)...);
        }
};
}  // namespace gel
#endif