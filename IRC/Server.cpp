#include"Server.hpp"

Server::Server(int ac, char **av){
    if(ac != 3){
        std::cerr << "Usage: ./server <port> <password>" << std::endl;
        exit ;
    }
    port = std::atoi(av[1]);
    password = av[2];
}
void Server::setupserver(){
    socketD = socket(AF_INET, SOCK_STREAM, 0);
    if (socketD < 0){
        std::cerr << "Failed to set up server\n";
        close(socketD);
        exit;
    }
    int opt = 1;
    if(setsockopt(socketD, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0){
        std::cerr << "failed to setsocketopt\n";
        exit;
    }
    struct sockaddr_in ser_addr;
    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = INADDR_ANY;
    ser_addr.sin_port = htons(port);

    if(bind(socketD, (struct sockaddr*)&ser_addr, sizeof(ser_addr)) < 0)
    {
        std::cerr << "failed to bind\n";
        close(socketD);
        exit;
    }
    if(listen(socketD, 5) < 0){
        std::cerr << "failed to listen\n";
        close(socketD);
        exit;
    }
    fcntl(socketD, F_SETFL, O_NONBLOCK);
    
}
void Server::run(){
    while(1){
        int pollcount = poll(&pollfds[0], pollfds.size(), 1)
        if(pollcount < 0){
            std::cerr << "failed to poll\n";
            break;
        }
    }
    for(size_t i = 0, i < pollfds.size(), i++){
        if(pollfds[i].revents & POLLIN){
            if(pollfds[i].fd == serverfd)
                acceptNewClient();
            else 
                handleClientMessage(pollfds[i].fd);
        }
    }
}
void Server::acceptNewClient(){
    sturct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int clientFd = accept(serverfd, (struct sockaddr*) &client_addr, &client_len);
    if(client  < 0){
        std::cerr << "failed to accept\n";
        return;
    }
    fcntl(clintFd, F_SETFL, O_NONBLOCK);
    pollFds.push_back(clientFd, POLLIN, 0);
    clients[clientFd] = "";
}

void Server::handleClientMessage(int clientfd){
    char buffer[512];
    int nbytes = recv(clientfd, buffer, sizeof(buffer)- 1, 0);
    if(nbytes <= 0){
        if(nbytes == 0)
        {
            std::cout << "Client disconnected: " << clientfd << std::endl;
            else{
                std::cerr << "failed recv\n";
            }
            removeclient(clientfd);
        }
        else{
            buffer[nbytes] = '\0';
            std::string message(buffer);
            std::cout << "Received message from client " << clientfd << ": " << message << std::endl;
            send(clientfd, message.c_str(), message.lenth(). 0);
        }
    }
}
void Server::removeclient(int clientfd){
    
}

Server::~Server()
{
    
}
