#include"Server.hpp"

int main(int ac, char **av)
{
    Server s(ac , av);
    s.setupserver();
    return 0;
}