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
        Server(int, const string &);
        ~Server();
        void run();
        void  setupserver();
        void acceptNewClient();
        void handleClientMessage(int);
        void removeclient(int);

    private:
        
        int socketD;
        int _port;
        string _password;
        vector<struct pollfd> pollfds;
        map <int , string> clients;
};
        