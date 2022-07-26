#include "cialloo/rcon.hpp"
#include <iostream>

int main()
{
    // cialloo::rcon::host h("182.61.27.111", "27015", "123456");
    cialloo::rcon::client client("182.61.27.111", "27015", "123456");
    std::cout << client.command("status") << std::endl;
    std::cout << client.command("cvarlist") << std::endl;
    std::cout << client.command("status") << std::endl;
    return 0;
}