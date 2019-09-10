#pragma once
#ifndef GEL_SIGNAL_SLOT_HPP
#define GEL_SIGNAL_SLOT_HPP
#include <functional>

namespace gel {
template <typename... Params>
class Slot {
       public:
        using Func_t = std::function<void(Params...)>;
        explicit Slot(const std::string& uuid, Func_t func);
        void call(Params... params);
        std::string getUUID() const;

       private:
        std::string uuid;
        Func_t func;
};

template <typename... Params>
inline Slot<Params...>::Slot(const std::string& uuid,
                             Slot<Params...>::Func_t func)
    : uuid(uuid), func(func) {}

template <typename... Params>
inline void Slot<Params...>::call(Params... params) {
        func(params...);
}
template <typename... Params>
inline std::string Slot<Params...>::getUUID() const {
        return uuid;
}
}  // namespace gel
#endif