#ifndef RCON
#define RCON

#include <string>
#include <asio.hpp>

namespace valve {

#define SERVERDATA_AUTH             3
#define SERVERDATA_AUTH_RESPONSE    2
#define SERVERDATA_EXECCOMMAND      2
#define SERVERDATA_RESPONSE_VALUE   0

#define RCON_WAIT_BUFFER_SIZE       1024
#define RCON_WAIT_BUFFER_TIME       500     // milisecond

static bool g_auth = false;
static unsigned int g_packet_id = 1;

std::string send_packet(std::string cmd, int32_t type, asio::ip::tcp::socket& socket);

int32_t byte32_to_int(unsigned char* array)
{
    return (static_cast<int32_t>((array[0] << 0) | (array[1] << 8) | (array[2] << 16) | (array[3] << 24)));
}

std::string process_command(int32_t packet_id, asio::ip::tcp::socket& socket)
{
    if(!g_auth)
        return "Authenticate fail!\nPlease check your rcon config.\n";

    send_packet("", SERVERDATA_RESPONSE_VALUE, socket);
    std::string response;
    int time = 0;
    while(1)
    {
        unsigned char size[4], id[4], type[4];
        socket.receive(asio::buffer(size));
        socket.receive(asio::buffer(id));
        socket.receive(asio::buffer(type));
        size_t content_size = (byte32_to_int(size) - 8);
        if(content_size > RCON_WAIT_BUFFER_SIZE)
            std::this_thread::sleep_for(std::chrono::milliseconds(RCON_WAIT_BUFFER_TIME));
        char* body = new char[content_size];
        socket.receive(asio::buffer(body, content_size));

        if(byte32_to_int(id) == (packet_id + 1) && time == 1)
        {
            delete body;
            break;
        }
        else if(byte32_to_int(id) == (packet_id + 1) && time != 1)
            time++;
        
        for(size_t i = 0; i < content_size; i++)
        {
            if(body[i] != '\0')
                response += body[i];
        }

        delete body;
    }

    return response;
}

std::string process_auth(int32_t id, asio::ip::tcp::socket& socket)
{
    unsigned char receive[14];
    socket.receive(asio::buffer(receive));
    socket.receive(asio::buffer(receive));
    if(id == byte32_to_int(receive + 4))
        g_auth = true;
    else 
        g_auth = false;
    return "";
}

std::string send_packet(std::string cmd, int32_t type, asio::ip::tcp::socket& socket)
{
    g_packet_id++;
    int32_t packet_size = 4 + 4 + 4 + cmd.length() + 1 + 1;
    unsigned char* packet = new unsigned char[packet_size];
    for(size_t i = 0; i < packet_size; i++)
        packet[i] = '\0';
    for(uint16_t i = 0; i < 4; i++)
        packet[i] = static_cast<unsigned char>((packet_size - 4) >> (i * 8));
    for(uint16_t i = 0; i < 4; i++)
        packet[4 + i] = static_cast<unsigned char>(g_packet_id >> (i * 8));
    for(uint16_t i = 0; i < 4; i++)
        packet[8 + i] = static_cast<unsigned char>(type >> (i * 8));
    for(size_t i = 0; i < cmd.length(); i++)
        packet[12 + i] = static_cast<unsigned char>(cmd[i]);
    socket.send(asio::buffer(packet, packet_size));
    delete packet;

    if(type == SERVERDATA_AUTH)
        return process_auth(g_packet_id, socket);
    else if(type == SERVERDATA_EXECCOMMAND)
        return process_command(g_packet_id, socket);

    return "";
}

void authenticate(std::string password, asio::ip::tcp::socket& socket)
{
    send_packet(password, SERVERDATA_AUTH, socket);
}

std::string rcon(std::string host, std::string port, std::string password, std::string cmd)
{
    asio::io_context io_context;
    asio::ip::tcp::socket socket(io_context);
    asio::ip::tcp::resolver resolver(io_context);
    asio::connect(socket, resolver.resolve(host, port));
    authenticate(password, socket);
    std::string response = send_packet(cmd, SERVERDATA_EXECCOMMAND, socket);
    socket.close();
    return response;
}

}

#endif