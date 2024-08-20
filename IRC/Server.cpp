#include"Server.hpp"

Server::Server(int port, string password) : password(password) {
   serverfd = setupserver(port);
}
int   Server::setupserver(int port){
    int socketD = socket(AF_INET, SOCK_STREAM, 0);
    if (socketD < 0){
        cerr << "Failed to set up server\n";
        close(socketD);
        exit(EXIT_FAILURE);
    }
    int opt = 1;
    if(setsockopt(socketD, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0){
        cerr << "Failed to setsocketopt\n";
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in sock_add;
    memset(&sock_add, 0, sizeof(sock_add));
    sock_add.sin_family = AF_INET;
    sock_add.sin_port = htons(port);
    sock_add.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(socketD, (struct sockaddr*)&sock_add, sizeof(sock_add)) < 0)
    {
        cerr << "Failed to bind\n";
        close(socketD);
        exit(EXIT_FAILURE);
    }
    else
        cout << "server id bind\n";
    if(listen(socketD, 5) < 0){
        cerr << "Failed to listen\n";
        close(socketD);
        exit(EXIT_FAILURE);
    }
    else 
        cout << "server is listen\n";
    
    cout << "Waiting for a connection..." << std::endl;
    // fcntl(socketD, F_SETFL, O_NONBLOCK);
    return(socketD);
}
void Server::run(){
    
    while(1){
        int pollcount = poll(&pollfds[0], pollfds.size(), 1);
        if(pollcount < 0){
            cerr << "Failed to poll\n";
            break; 
        }
    }
    for(size_t i = 0; i < pollfds.size(); i++){
        cout << "New connection for the socket :" << serverfd << endl;
        if(pollfds[i].revents & POLLIN){
            if(pollfds[i].fd == serverfd)
            {
                acceptNewClient();
                cout << "New connection for the socket :" << serverfd << endl;
            }
            else 
                handleClientMessage(pollfds[i].fd);
        }
    }
}
void Server::acceptNewClient(){
    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    socklen_t client_len = 0;
    int clientFd = accept(serverfd, (struct sockaddr*) &client_addr, &client_len);
    if(clientFd  < 0){
        cerr << "Failed to accept\n";
        return;
    }
    else 
        cout << "server accept client >> " << clientFd << endl;
    // fcntl(clientFd, F_SETFL, O_NONBLOCK);
    // struct pollfd pfd;
    // pfd.fd = clientFd;
    // pfd.events = POLLIN;
    // pfd.revents = 0;
    // pollfds.push_back(pfd);
    // clients[clientFd] = "";
}

void Server::handleClientMessage(int clientfd){
    char buffer[512];
    memset(buffer, 0, sizeof(buffer));

    int nbytes = recv(clientfd, buffer, sizeof(buffer)- 1, 0);
    if(nbytes <= 0){
        if(nbytes == 0)
            cout << "Client disconnected: " << clientfd << endl;
        else
                cerr << "Failed recv\n";
            removeclient(clientfd);
        }
    else{
        buffer[nbytes] = '\0';
        string message(buffer);
        cout << "Received message from client " << clientfd << ": " << message << endl;
        send(clientfd, message.c_str(), message.length(),0);
    }
}

void Server::removeclient(int clientfd){
    close(clientfd);
    for(vector<struct pollfd>::iterator it = pollfds.begin(); it != pollfds.end(); it++){
        if(it->fd == clientfd){
            pollfds.erase(it);
            break;
        }
    }
    clients.erase(clientfd);
}

Server::~Server(){
    
}
