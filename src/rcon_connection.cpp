#include "cialloo/rcon.hpp"
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <chrono>
#include <thread>
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

connection::~connection()
{
    socket_.close();
}

void connection::authenticate(std::string password)
{
    send_packet(password, SERVERDATA_AUTH);
}

std::string connection::send_packet(std::string cmd, int32_t type)
{
    packet_id_++;
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
    else if(type == SERVERDATA_EXECCOMMAND)
        return process_command(connection::packet_id_);

    return "";
}

std::string connection::process_command(int32_t packet_id)
{
    if(!auth_)
        return "Authenticate fail!\nPlease check your rcon config.\n";

    send_packet("", SERVERDATA_RESPONSE_VALUE);
    std::string response;
    int time = 0;
    while(1)
    {
        unsigned char size[4], id[4], type[4];
        socket_.receive(net::buffer(size));
        socket_.receive(net::buffer(id));
        socket_.receive(net::buffer(type));
        size_t content_size = (byte32_to_int(size) - 8);
        if(content_size > RCON_WAIT_BUFFER_SIZE)
            std::this_thread::sleep_for(std::chrono::milliseconds(RCON_WAIT_BUFFER_TIME));
        char body[content_size];
        socket_.receive(net::buffer(body, content_size));
        if(byte32_to_int(id) == (packet_id + 1) && time == 1)
            break;
        else if(byte32_to_int(id) == (packet_id + 1) && time != 1)
            time++;
        for(size_t i = 0; i < content_size; i++)
        {
            if(body[i] != '\0')
                response += body[i];
        }
    }

    return response;
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