#include "cialloo/rcon.hpp"

namespace cialloo {
namespace rcon {

client::client(host h) : connection_(h)
{
}

client::client(std::string addr, std::string port, std::string password)
    : connection_(host(addr, port, password))
{
}

client::~client()
{
}

std::string client::command(std::string cmd)
{
    return connection_.send_packet(cmd, SERVERDATA_EXECCOMMAND);
}

} // namespace rcon
} // namespace cialloo