#ifndef CIALLOO_RCON_HPP
#define CIALLOO_RCON_HPP

#include "cialloo/rcon_host.hpp"
#include "cialloo/rcon_connection.hpp"

#define SERVERDATA_AUTH             3
#define SERVERDATA_AUTH_RESPONSE    2
#define SERVERDATA_EXECCOMMAND      2
#define SERVERDATA_RESPONSE_VALUE   0

namespace cialloo {
namespace rcon {
    
namespace net = boost::asio;

class rcon
{
    connection connection_;
public:
    rcon(host h);
    ~rcon();
    rcon& operator=(const rcon&) = delete;
    rcon(const rcon&) = delete;

    /**
     * Send a command to target host
     * 
     * @param cmd       command such as status or changelevel
     * @return          server's return value
     */
    std::string command(std::string cmd);
};


} // namespace rcon
} // namespace cialloo

#endif // CIALLOO_RCON_HPP