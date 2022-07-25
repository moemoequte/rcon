#ifndef CIALLOO_RCON_CONNECTION_HPP
#define CIALLOO_RCON_CONNECTION_HPP

#include <stdint.h>
#include <boost/asio.hpp>
#include "cialloo/rcon.hpp"

namespace cialloo {
namespace rcon {

namespace net = boost::asio;

class connection
{
    net::io_context io_context_;
    net::ip::tcp::socket socket_;
    net::ip::tcp::resolver resolver_;
public:
    connection(host h);
    ~connection(){};
    connection& operator=(const connection&) = delete;
    connection(const connection&) = delete;

    static int32_t packet_id_;

    /**
     * Send a rcon structure packet to server
     * 
     * @param cmd       command
     * @param type      packet type
     * @return          server's return value
     */
    std::string send_packet(std::string cmd, int32_t type);

private:
    void authenticate(std::string password);
};


} // namespace rcon
} // namespace cialloo

#endif // CIALLOO_RCON_CONNECTION_HPP