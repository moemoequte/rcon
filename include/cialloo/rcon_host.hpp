#ifndef CIALLOO_RCON_HOST_HPP
#define CIALLOO_RCON_HOST_HPP

#include <string>

namespace cialloo {
namespace rcon {

struct host
{
    std::string address;        // remote source server's ip or domain
    std::string port;           // rcon port
    std::string password;       // rcon password

    host(std::string addr, std::string p, std::string pass)
        : address(addr),
          port(p),
          password(pass){}
};


} // rcon
} // cialloo


#endif // CIALLOO_RCON_HOST_HPP