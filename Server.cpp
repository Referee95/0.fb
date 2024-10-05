#include "Server.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Server::Server()
{
	
}

Server::Server(int port, const string &password) : _port(port), _password(password), socketD(-1)
{
}

Server::Server( const Server & src )
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Server::~Server()
{    
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Server &				Server::operator=( Server const & rhs )
{
	if ( this != &rhs )
	{
		this->_port = rhs._port;
		this->_password = rhs._password;
		this->socketD = rhs.socketD;
		this->pollfds = rhs.pollfds;
		this->_users = rhs._users;
		this->_channels = rhs._channels;
		this->_maxFd = rhs._maxFd;
		this->_masterSet = rhs._masterSet;
		this->_readSet = rhs._readSet;
		this->_authenticated = rhs._authenticated;
		this->_clients = rhs._clients;
	}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Server const & i )
{
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/


User *Server::findUser(string &name)
{
	map<int, User *>::iterator it = _users.begin();
	while (it != _users.end())
	{
		User *user = it->second;
		if (user->getNickName() == name)
			return user;
		++it;
	}
	return NULL;
}

void	Server::start(string port, string password)
{
	std::cout << "Server started" << std::endl;
	_password = password;
	_port = atoi(port.c_str());
	run();
}

void Server::_hundleMessage(string message, int clientFd)
{

	std::istringstream iss(message);
	string command;
	User *user;
	user = _users[clientFd];
	int x = 0;
	// cout << "message: " << message << endl;
	while (iss >> command)
	{
		// cout << "command: " << command << endl;
		// cout << x << endl;
		if (command == "CAP" || command == "LS"){
			x++;
			continue ;
		}
		else if (command == "PASS")
		{
			x++;
			string password;
			iss >> password;
			if (password != _password)
			{
				std::stringstream ss;
				ss << user->getFd();
				string response = ss.str() + ": 464 The password is incorrect\r\n";
				send(clientFd, response.c_str(), response.size(), 0);
				break ;
			}
			else if (user->getSigned() == true)
			{
				std::stringstream ss;
				ss << user->getFd();
				string response = ss.str() + ": 462 You may not reregister\r\n";
				send(clientFd, response.c_str(), response.size(), 0);
			}
			else
			{
				user->setSigned(true);
				std::stringstream ss;
				ss << (user->getFd() - 3);
				string response = "ID "  + ss.str() + ": 001 Welcome to the 0.Facebook\r\n";
				send(clientFd, response.c_str(), response.size(), 0);
			}
		}
		else if (command == "NICK")
		{
			x++;
			string nickName;
			iss >> nickName;
			string msgNick = ":" + user->getNickName() + "!" + user->getUserName() + "@" + "0.0.0.0.IP NICK :" + nickName + "\r\n";
			user->setNickName(nickName);
			send(clientFd, msgNick.c_str(), msgNick.size(), 0);
			user->setNick(true);
		}
		else if (command == "USER")
		{
			x++;
			string userName, realName, ipAdresse, star ,zero;
			iss	>> userName;
			user->setUserName(userName);
			iss >> zero;
			iss >> star;
			iss >> realName;
			while (iss >> realName)
				realName += " " + realName;
			user->setRealName(realName);
			iss >> ipAdresse;
			user->setIpAdresse(ipAdresse);
			user->setUser(true);
		}
		else if (command == "JOIN")
		{
			x++;
			if (user->getSigned() == false || user->getNick() == false || user->getUser() == false)
			{
				string msg = user->getNickName() + " " + command + " : 451 You have not registered\r\n";
				send(clientFd, msg.c_str(), msg.size(), 0);
				break ;
			}
			string channelName;
			iss >> channelName;
			if (channelName[0] != '#')
			{
				string msg = user->getNickName() + " " + command + " : 403 Invalid channel name\r\n";
				send(clientFd, msg.c_str(), msg.size(), 0);
				break ;
			}
			bool newChannel = (_channels.find(channelName) == _channels.end());
			if (newChannel)
			{
				_channels[channelName] = Channel(channelName);
				Channel& channel = _channels[channelName];
				channel.addUser(user);
				channel.addAdmin(user);
				user->setOperator(true);
				
			}
			else
			{
				Channel& channel = _channels[channelName];
				if (channel.getHasPass())
				{
					string pass;
					iss >> pass;
					if (pass != channel.getPass())
					{
						string msg = ":0.facebook 475 " + user->getNickName() + " " + channelName + " :Cannot join channel (+k)\r\n";
						send(clientFd, msg.c_str(), msg.size(), 0);
						break ;
					}
				}
				channel.addUser(user);
			}
			string joinMsg = ":" + user->getNickName() + "!" + user->getUserName() + "@" + user->getIpAdresse() + ".IP JOIN " + channelName + " * :" + user->getRealName() + "\r\n";
			send(clientFd, joinMsg.c_str(), joinMsg.size(), 0);
		}
		else if(command == "KICK")
		{
			string skip, channelName, nick;
			x++;
			std::stringstream iss(message);
			iss >> skip >> channelName >> nick;

			Channel &chan = _channels[channelName];
			bool check = false;
			std::vector<User *>::iterator it = chan.getUsers().begin();
			for (; it != chan.getUsers().end(); it++) {
				if ((*it)->getNickName() == nick) {
					check = true;
					break;
				}
			}
			if (check){
				chan.removeUser(*it);
				string kickMsg = ":" + user->getNickName() + "!" + user->getUserName() + "@" + user->getIpAdresse() + ".IP KICK " + channelName + " " + nick + "\r\n";
				send(clientFd, kickMsg.c_str(), kickMsg.size(), 0);
			}
			else{
				string response = ":0.facebook 401 " + channelName + " " +  nick + " :No such nick\r\n";
				send(clientFd, response.c_str(), response.size(), 0);
			}
		}
		else if (command == "PRIVMSG")
		{
			x++;
			string recipent;
			iss >> recipent;
			if (recipent[0] == '#')
			{
				if (_channels.find(recipent) != _channels.end())
				{
					string msg;
					iss >> msg;
					Channel &channel = _channels[recipent];
					vector<User *> users = channel.getUsers();

					for (vector<User *>::iterator it = users.begin();  it != users.end(); it++)
					{
						User *user = *it;
						string message = ":" + user->getNickName() + "!" + user->getUserName() + "@" + user->getIpAdresse() + ".IP PRIVMSG " + recipent + " " + msg.substr(1) + "\r\n";
						send(user->getFd(), message.c_str(), message.size(), 0);
					}
				}
			}
			else
			{
				map<int , User *>::iterator it = _users.begin();
				while (it != _users.end())
				{
					User *use = it->second;
					if(use->getNickName() == recipent)
					{
						string message;
						iss >> message;
						string prvMsg = ":" + user->getNickName() + "!" + user->getUserName() + "@" + user->getIpAdresse() + ".IP PRIVMSG " + recipent + " " +  message + "\r\n";
						send(use->getFd(), prvMsg.c_str(), prvMsg.size(), 0);
					}
					++it;
				}
			}
		}
		else if (command == "WHO")
		{
			string channelName;
			iss >> channelName;

			if (_channels.find(channelName) != _channels.end())
			{
				x++; 
				Channel &channel = _channels[channelName];
				vector<User *> users = channel.getUsers();
				string msg = ":127.0.0.1 353 " + user->getNickName() + " = " + channelName + " :";
				for(vector<User *>::iterator it = users.begin() ; it != users.end() ; ++it)
				{
					User *use = *it;
					msg = msg + use->getNickName() + " ";
				}
				msg = msg + "\r\n";
				send(user->getFd(), msg.c_str(), msg.size(), 0);
				for(vector<User *>::iterator it = users.begin() ; it != users.end() ; ++it)
				{
					User *use = *it;
					string sendNickName = ":127.0.0.1 354 " + user->getNickName() + " 152 " + channelName + " " + use->getNickName() + " " + use->getIpAdresse() + ".IP * .127.0.0.1";

				}
				string endList;
				endList = ":127.0.0.1 315 " + user->getNickName() + " " + channelName + " :End of /WHO list.\r\n";
				send(user->getFd(), endList.c_str(), endList.size(), 0);
			}
		}
		else if (command == "MODE")
		{
			x++;
			string channelName , mode, argument, reponse;
			iss >> channelName >> mode >> argument;
			if (_channels.find(channelName) != _channels.end())
			{
				Channel &channel = _channels[channelName];
				if(mode.empty())
				{
					reponse = ":127.0.0.1 324 " + user->getNickName() + " " + channelName + " +tn\r\n";
					send(clientFd, reponse.c_str(), reponse.size(), 0);
					reponse = ":127.0.0.1 329 " + user->getNickName() + " " + channelName + " 1726572593\r\n";
					send(clientFd, reponse.c_str(), reponse.size(), 0);
				}
				else if(mode == "+i")
				{
					channel.setInviteOnly(true);
					channel.addAdmin(user);
					reponse = ":" + user->getNickName() + "!" + user->getUserName() + "@" + "0.0.0.0.IP MODE " + channelName + " +i\r\n";
					send(clientFd, reponse.c_str(), reponse.size(), 0);
				}
				else if(mode == "-i"){
					channel.setInviteOnly(false);
					channel.removeAdmin(user);
					reponse = ":" + user->getNickName() + "!" + user->getUserName() + "@" + "0.0.0.0.IP MODE " + channelName + " -i\r\n";
					send(clientFd, reponse.c_str(), reponse.size(), 0);
				}
				else if(mode == "+t"){
					channel.setHasTopic(true);
					channel.setTopic(argument);
					reponse = ":" + user->getNickName() + "!" + user->getUserName() + "@0.0.0.0.IP MODE " + channelName + " +t\r\n";
					send(clientFd, reponse.c_str(), reponse.size(), 0);
				}
				else if(mode == "-t"){
					channel.setHasTopic(false);
					channel.setTopic("");
					reponse = ":" + user->getNickName() + "!" + user->getUserName() + "@0.0.0.0.IP MODE " + channelName + " -t\r\n";
					send(clientFd, reponse.c_str(), reponse.size(), 0);
				}
				else if(mode == "+k"){
					channel.setHasPass(true);
					if (argument.empty())
					{
						string msg = "0.facebook 461 " + user->getNickName() + " MODE +k :Not enough parameters\r\n";
						send(clientFd, msg.c_str(), msg.size(), 0);
					}
					channel.setPass(argument);
				}
				else if(mode == "-k"){
					channel.setHasPass(false);
					channel.setPass("");
				}
				else if(mode == "+o")
				{
					if (argument.empty())
					{
						string msg = "0.facebook 461 " + user->getNickName() + " MODE +o :Not enough parameters\r\n";
						send(clientFd, msg.c_str(), msg.size(), 0);
					}
					else
					{
						if (!user->getOperator())
						{
							string msg = ":0.facebook 482 " + user->getNickName() + " " + channelName + " :You're not channel operator\r\n";
							send(user->getFd(), msg.c_str(), msg.size(), 0);
							break;
						}
						else
						{
							User *newAdmin = findUser(argument);
							if (newAdmin == NULL)
							{
								string msg = ":0.facebook 401 " + user->getNickName() + " " + argument + " :No such nick\r\n";
								send(user->getFd(), msg.c_str(), msg.size(), 0);
								break;
							}
							else
							{
								channel.addAdmin(newAdmin);
								string msg = ":" + user->getNickName() + "!" + user->getUserName() + "@" + user->getIpAdresse() + ".IP MODE " + channelName + " +o " + argument + "\r\n";
								send(clientFd, msg.c_str(), msg.size(), 0);
							}

						}
					}
				}
				else if(mode == "-o"){
					if (argument.empty())
					{
						string msg = "0.facebook 461 " + user->getNickName() + " MODE -o :Not enough parameters\r\n";
						send(clientFd, msg.c_str(), msg.size(), 0);
					}
					else
					{
						if (!user->getOperator())
						{
							string msg = ":0.facebook 482 " + user->getNickName() + " " + channelName + " :You're not channel operator\r\n";
							send(user->getFd(), msg.c_str(), msg.size(), 0);
							break;
						}
						else
						{
							User *newAdmin = findUser(argument);
							if (newAdmin == NULL)
							{
								string msg = ":0.facebook 401 " + user->getNickName() + " " + argument + " :No such nick\r\n";
								send(user->getFd(), msg.c_str(), msg.size(), 0);
								break;
							}
							else
							{
								channel.removeAdmin(newAdmin);
								string msg = ":" + user->getNickName() + "!" + user->getUserName() + "@" + user->getIpAdresse() + ".IP MODE " + channelName + " -o " + argument + "\r\n";
								send(clientFd, msg.c_str(), msg.size(), 0);
							}
						}
					}
				}
				else if(mode == "+l"){
					channel.setUserLimit(atoi(argument.c_str()));
					cout << "limit: " << channel.getUserLimit() << endl;
				}
				else if(mode == "-l"){
					channel.setUserLimit(0);
					cout << "limit: " << channel.getUserLimit() << endl;
				}
			}
		}
		else if (command == "INVITE")
		{
			x++;
			string channelName, invitedNick;
			iss >> invitedNick >> channelName;
			if (_channels.find(channelName) == _channels.end())
			{
				string msg = user->getNickName() + " " + command + " : 403 No such channel\r\n";
        		send(clientFd, msg.c_str(), msg.size(), 0);
        		break;
			}
			Channel &channel = _channels[channelName];
			if (!user->getOperator() || !channel.getInviteOnly())
			{
				string msg = user->getNickName() + " " + command + " : 482 You're not channel operator\r\n";
				send(clientFd, msg.c_str(), msg.size(), 0);
				break;
			}
			User *invited = findUser(invitedNick);
			if (invited == NULL)
			{
				string msg = user->getNickName() + " " + command + " : 401 No such nick\r\n";
				send(clientFd, msg.c_str(), msg.size(), 0);
				break;
			}
			channel.addInvited(invited);
			int invitedFd = invited->getFd();
			string inviteMsg = ":" + user->getNickName() + "!" + user->getUserName() + "@" + user->getIpAdresse() + ".IP INVITE " + invitedNick + " " + channelName + "\r\n";
			send(invitedFd, inviteMsg.c_str(), inviteMsg.size(), 0);
		}
		else if (command == "TOPIC")
		{
			x++;
			string channelName, topic;
			iss >> channelName;
			if (_channels.find(channelName) == _channels.end())
			{
				string msg = user->getNickName() + " " + command + " : 403 No such channel\r\n";
        		send(clientFd, msg.c_str(), msg.size(), 0);
        		break;
			}
			Channel &channel = _channels[channelName];
			if (!user->getOperator() || !channel.getHasTopic())
			{
				string msg = ":127.0.0.1 482 " + user->getNickName() + " " + channelName + " :You're not channel operator\r\n";
				send(user->getFd(), msg.c_str(), msg.size(), 0);
				break;
			}
			std::getline(iss, topic);
			if (topic[0] == ' ' && topic[1] == ':')
				topic = topic.substr(2);
			channel.setTopic(topic);
			string msg = ":" + user->getNickName() + "!" + user->getUserName() + "@" + user->getIpAdresse() + ".IP TOPIC " + channelName + " :" + topic + "\r\n";
			send(clientFd, msg.c_str(), msg.size(), 0);
		}
		else if (command == "QUIT")
		{
			x++;
			string msg;
			iss >> msg;
			string quitMsg = ":" + user->getNickName() + "!" + user->getUserName() + "@" + user->getIpAdresse() + ".IP QUIT :" + msg + "\r\n";
			send(clientFd, quitMsg.c_str(), quitMsg.size(), 0);
			removeclient(clientFd);
		}
		else if (command == "PART")
		{
			x++;
			string channelName;
			iss >> channelName;
			if (_channels.find(channelName) != _channels.end())
			{
				Channel &channel = _channels[channelName];
				channel.removeUser(user);
				string partMsg = ":" + user->getNickName() + "!" + user->getUserName() + "@" + user->getIpAdresse() + ".IP PART " + channelName + "\r\n";
				send(clientFd, partMsg.c_str(), partMsg.size(), 0);
			}
		}
		else if(x == 0)
		{
			string msg = ":127.0.0.1 421 " + user->getNickName() + " " + command + " :Unknown command\r\n";
			cout << msg << endl;
			send(clientFd, msg.c_str(), msg.size(), 0);
			return ;
		}
	}


	// // if (command == "kick")
	// // {
	// 	std::string namekicker = "John";
	// 	std::string kickmssg = ":"  "mayache- KICK " + channelName + " " + namekicker + "\r\n";
	// 	kick(*user, kickmssg);
	// 	 // :WiZ!jto@tolsun.oulu.fi KICK #Finnish John
	// // }
}


void _checkRegistredUser()
{

}
/*
** --------------------------------- ACCESSOR ---------------------------------
*/


void   Server::setupserver()
{
    socketD = socket(AF_INET, SOCK_STREAM, 0);
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
    sock_add.sin_port = htons(_port);
    sock_add.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(socketD, (struct sockaddr*)&sock_add, sizeof(sock_add)) < 0)
    {
        cerr << "Failed to bind\n";
        close(socketD);
        exit(EXIT_FAILURE);
    }
    else
        cout << "server is bind\n";
    if(listen(socketD, 5) < 0){
        cerr << "Failed to listen\n";
        close(socketD);
        exit(EXIT_FAILURE);
    }
    else 
        cout << "server is listen\n";
    
    fcntl(socketD, F_SETFL, O_NONBLOCK);

    struct pollfd serverpfd;
    serverpfd.fd = socketD;
    serverpfd.events = POLLIN;
    pollfds.push_back(serverpfd);
    cout << "Waiting for a connection..." << std::endl;
}
void Server::run()
{
    setupserver();
    while(1){
        int pollcount = poll(&pollfds[0], pollfds.size(), -1);
        if(pollcount < 0){
            cerr << "Failed to poll\n";
            break; 
        }
        for(size_t i = 0; i < pollfds.size(); i++){
            if(pollfds[i].revents & POLLIN){
                if(pollfds[i].fd == socketD)
                    acceptNewClient();
                else 
                    handleClientMessage(pollfds[i].fd);
            }
        }
    }
}

void Server::acceptNewClient(){
    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    socklen_t client_len = 0;
    int clientFd = accept(socketD, (struct sockaddr*) &client_addr, &client_len);
    if(clientFd  < 0){
        cerr << "Failed to accept\n";
        return;
    }
    else 
        cout << "server accept client >> " << clientFd << endl;
    fcntl(clientFd, F_SETFL, O_NONBLOCK);
        struct pollfd pfd;
        pfd.fd = clientFd;
        pfd.events = POLLIN | POLLOUT;
        pollfds.push_back(pfd);
		User* newUser = new User(clientFd, "random" + std::to_string(clientFd), "random");
        _users[clientFd] = newUser;
}

void Server::handleClientMessage(int clientfd){
    char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    int nbytes = recv(clientfd, buffer, sizeof(buffer)- 1, 0);

    if(nbytes <= 0)
            removeclient(clientfd);
    else{
        buffer[nbytes] = '\0';
        string message = buffer;
		_hundleMessage(message, clientfd);
    }
}

void Server::removeclient(int clientfd)
{
    close(clientfd);
    for(vector<struct pollfd>::iterator it = pollfds.begin(); it != pollfds.end(); it++){
        if(it->fd == clientfd){
            pollfds.erase(it);
            cout << "Client disconnected: " << clientfd << endl;
            break;
        }
    }
    // clients.erase(clientfd);
}



/* ************************************************************************** */


// topic__ = true 


// if topic true 
// // rie operator liy9der ibdel

// else
// kolchu iy9der ibdel
