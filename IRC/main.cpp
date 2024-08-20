#include "Server.hpp"


int main(int ac, char **av)
{
    if(ac != 3){
        cerr << "Usage: ./ircserv <port> <password>" << std::endl;
        exit (EXIT_FAILURE);
    }
    int port = atoi(av[1]);
    string password = av[2];
    Server server(port, password);
    server.run();
      
}