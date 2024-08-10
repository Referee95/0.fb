#pragma once

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/poll.h>
#include <vector>
#include <map>


class Server {
    public:
        Server(int, char **);
        ~Server();
        void run();
        void setupserver();
        void acceptNewClient();
        void handleClientMessage(int);
        void removeclient(int);
    private:
        int socketD;
        int port;
        int serverfd;
        std::string password;
        std::vector<struct pollfd> pollfds;
        std::map <int , std::string> clients;
};
        