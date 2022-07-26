#ifndef CIALLOO_RCON_HPP
#define CIALLOO_RCON_HPP

#include "cialloo/rcon_host.hpp"
#include "cialloo/rcon_connection.hpp"

#define SERVERDATA_AUTH             3
#define SERVERDATA_AUTH_RESPONSE    2
#define SERVERDATA_EXECCOMMAND      2
#define SERVERDATA_RESPONSE_VALUE   0

#define RCON_WAIT_BUFFER_SIZE       1024
#define RCON_WAIT_BUFFER_TIME       500     // milisecond

namespace cialloo {
namespace rcon {
    
namespace net = boost::asio;

class client
{
    connection connection_;
public:
    client(host h);
    client(std::string host, std::string port, std::string password);
    ~client();
    client& operator=(const client&) = delete;
    client(const client&) = delete;

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