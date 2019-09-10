#pragma once
#ifndef GEL_SIGNAL_CONNECTION_HPP
#define GEL_SIGNAL_CONNECTION_HPP
#include "IConnectable.hpp"

namespace gel {
class Connection {
       public:
        explicit Connection(const std::string& uuid, IConnectable& con);
        void disconnect();

       private:
        IConnectable& con;
        std::string uuid;
};
}  // namespace gel
#endif