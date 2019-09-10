#pragma once
#ifndef GEL_SIGNAL_ICONNECTABLE_HPP
#define GEL_SIGNAL_ICONNECTABLE_HPP
#include <string>

namespace gel {
class IConnectable {
       public:
        IConnectable() = default;
        ~IConnectable() = default;
        virtual void disconnect(const std::string& uuid) = 0;

       private:
};
}  // namespace gel
#endif