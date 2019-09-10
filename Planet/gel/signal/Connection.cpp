#include "Connection.hpp"
namespace gel {

Connection::Connection(const std::string& uuid, IConnectable& con)
    : con(con), uuid(uuid) {}

void Connection::disconnect() { con.disconnect(uuid); }
}  // namespace gel
