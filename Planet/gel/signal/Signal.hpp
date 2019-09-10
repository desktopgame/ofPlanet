#pragma once
#ifndef GEL_SIGNAL_SIGNAL_HPP
#define GEL_SIGNAL_SIGNAL_HPP
#include <algorithm>
#include <functional>
#include <stdexcept>
#include "Connection.hpp"
#include "Slot.hpp"
#if _WIN32
#include <Objbase.h>
#include <Windows.h>
#endif

namespace gel {
template <typename... Params>
class Signal : public IConnectable {
       public:
        using Slot_t = Slot<Params...>;
        explicit Signal();
        Connection connect(typename Signal<Params...>::Slot_t::Func_t func);
        void disconnect(const std::string& uuid) override;
        void operator()(Params... params);

       private:
        std::string generate();
        std::vector<Slot_t> slots;
};
template <typename... Params>
inline Signal<Params...>::Signal() : slots() {}
template <typename... Params>
inline Connection Signal<Params...>::connect(
    typename Signal<Params...>::Slot_t::Func_t func) {
        Slot_t slot = Slot_t(generate(), func);
        Connection con = Connection(slot.getUUID(), *this);
        slots.push_back(slot);
        return con;
}
template <typename... Params>
inline void Signal<Params...>::disconnect(const std::string& uuid) {
        auto iter = std::remove_if(
            slots.begin(), slots.end(),
            [uuid](Slot_t slot) { return uuid == slot.getUUID(); });
        slots.erase(iter, slots.end());
}
template <typename... Params>
inline void Signal<Params...>::operator()(Params... params) {
        auto iter = slots.begin();
        while (iter != slots.end()) {
                Slot_t slot = *iter;
                slot.call(params...);
                ++iter;
        }
}
// private
template <typename... Params>
inline std::string Signal<Params...>::generate() {
#if _WIN32
        // https://kagasu.hatenablog.com/entry/2019/02/20/220832
        GUID guid;
        CoCreateGuid(&guid);
        char guid_cstr[37];
        snprintf(guid_cstr, sizeof(guid_cstr),
                 "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x", guid.Data1,
                 guid.Data2, guid.Data3, guid.Data4[0], guid.Data4[1],
                 guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5],
                 guid.Data4[6], guid.Data4[7]);
        return std::string(guid_cstr);
#else
        throw std::logic_error("Not supported OS");
#endif
}
}  // namespace gel
#endif