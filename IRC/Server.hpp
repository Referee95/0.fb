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

using std::string;
using std::cout;
using std::cerr;
using std::cin;
using std::endl;
using std::map;
using std::vector;


class Server {
    public:
        Server(int, string);
        ~Server();
        void run();
        int  setupserver(int);
        void acceptNewClient();
        void handleClientMessage(int);
        void removeclient(int);

    private:
        
        int serverfd;
        string password;
        vector<struct pollfd> pollfds;
        map <int , string> clients;
};
        