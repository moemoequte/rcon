#include "cialloo/rcon.hpp"
#include <string>
#include <boost/asio.hpp>
#include "cialloo/rcon_connection.hpp"

namespace cialloo {
namespace rcon {

int32_t connection::packet_id_ = 1;

connection::connection(host h)
    : socket_(io_context_),
      resolver_(io_context_)
{
    net::connect(socket_, resolver_.resolve(h.address, h.port));
    authenticate(h.password);
}

void connection::authenticate(std::string password)
{
    send_packet(password, SERVERDATA_AUTH);
}

std::string connection::send_packet(std::string cmd, int32_t type)
{
    int32_t packet_size = 4 + 4 + 4 + cmd.length() + 1 + 1;
    char packet[packet_size];
    packet[0] = packet_size - 4;
    packet[4] = ++connection::packet_id_;
    packet[8] = type;
    for(size_t i = 0; i < cmd.length(); i++)
        packet[12 + i] = cmd[i];
    packet[packet_size - 2] = packet[packet_size - 1] = '\0';
    socket_.send(net::buffer(packet, packet_size));
}

} // namespace rcon
} // namespace cialloo