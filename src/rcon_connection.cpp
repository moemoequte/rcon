#include "cialloo/rcon.hpp"
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "cialloo/rcon_connection.hpp"

namespace cialloo {
namespace rcon {

int32_t connection::packet_id_ = 1;

connection::connection(host h)
    : socket_(io_context_),
      resolver_(io_context_),
      auth_(false)
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
    unsigned char packet[packet_size];
    for(size_t i = 0; i < packet_size; i++)
        packet[i] = '\0';
    for(uint16_t i = 0; i < 4; i++)
        packet[i] = static_cast<unsigned char>((packet_size - 4) >> (i * 8));
    for(uint16_t i = 0; i < 4; i++)
        packet[4 + i] = static_cast<unsigned char>(connection::packet_id_ >> (i * 8));
    for(uint16_t i = 0; i < 4; i++)
        packet[8 + i] = static_cast<unsigned char>(type >> (i * 8));
    for(size_t i = 0; i < cmd.length(); i++)
        packet[12 + i] = static_cast<unsigned char>(cmd[i]);
    socket_.send(net::buffer(packet, packet_size));

    if(type == SERVERDATA_AUTH)
        return process_auth(connection::packet_id_);

    return "";
}

std::string connection::process_auth(int32_t id)
{
    unsigned char receive[14];
    socket_.receive(net::buffer(receive));
    socket_.receive(net::buffer(receive));
    if(id == byte32_to_int(receive + 4))
        auth_ = true;
    else 
        auth_ = false;
    return "";
}

int32_t connection::byte32_to_int(unsigned char* array)
{
    return (static_cast<int32_t>((array[0] << 0) | (array[1] << 8) | (array[2] << 16) | (array[3] << 24)));
}

} // namespace rcon
} // namespace cialloo