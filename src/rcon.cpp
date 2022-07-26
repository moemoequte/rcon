#include "cialloo/rcon.hpp"

namespace cialloo {
namespace rcon {

rcon::rcon(host h) : connection_(h)
{
}

rcon::rcon(std::string addr, std::string port, std::string password)
    : connection_(host(addr, port, password))
{
}

rcon::~rcon()
{
}

std::string rcon::command(std::string cmd)
{
    return connection_.send_packet(cmd, SERVERDATA_EXECCOMMAND);
}

} // namespace rcon
} // namespace cialloo