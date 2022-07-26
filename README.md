## Source RCON Protocol
[Introduction](https://developer.valvesoftware.com/wiki/Source_RCON_Protocol)

Usage:
```cpp
#include "cialloo/rcon.hpp"
#include <iostream>

int main()
{
    cialloo::rcon::host h("127.0.0.1", "27015", "password");
    cialloo::rcon::rcon client(h);
    std::cout << client.command("status") << std::endl;
    std::cout << client.command("cvarlist") << std::endl;
    std::cout << client.command("status") << std::endl;
    return 0;
}
```