# RCON
The Source RCON Protocol(https://developer.valvesoftware.com/wiki/Source_RCON_Protocol) implementation in cplusplus, depend on asio library.
Code example:
```cpp
#include <iostream>
#include <rcon.hpp>

int main()
{
    std::cout << rcon("127.0.0.1", "27015", "password", "echo hello world");
    return 0;
}
```