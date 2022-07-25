#include "cialloo/rcon.hpp"

int main()
{
    cialloo::rcon::host h("182.61.27.111", "27015", "123456");
    cialloo::rcon::rcon client(h);
    return 0;
}