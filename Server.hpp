#ifndef SERVER_HPP
# define SERVER_HPP

#include <cstdlib>  
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/poll.h>
// #include "Client.hpp"
// class

# include "User.hpp"
# include "Channel.hpp"

# include <iostream>
# include <string>
# include <netinet/in.h>
# include <sys/socket.h>
# include <cstring>
# include <unistd.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <map>
# include <fcntl.h>
# include <vector>
# include <sstream>


using std::string;
using std::cout;
using std::cerr;
using std::cin;
using std::endl;
using std::map;
using std::vector;



class Server
{

	public:
	//  added by 7akam
	    Server(int, const string &);
		void run();
		void  setupserver();
		void acceptNewClient();
		void handleClientMessage(int);
		void removeclient(int);	
		// void check_buffer(string, int);

		std::vector<User>		user;
		std::vector<Channel>	channels;

		Server();
		Server( Server const & src );
		~Server();
		void start(string port, string password);

		Server &		operator=( Server const & rhs );	
		void 	kick(User &sender, std::string buffer);
		void	send_err(int code, User &sender, std::string arg1, std::string msg);

		// mayache added // Commands 
		// void kick(User &sender, std::string buffer);
		// void	join(User &sender, std::string buffer);
		int	send_msg(int dest_fd, std::string msg);
		Channel*	find_channel(std::string name);
		User*	findUser(std::string &name);


		
		// void setServerName(const std::string& name) {
		// 	Server_name = name;
		// }

		// void setIP(const std::string& ip) {
		// 	IP = ip;
		// }
		

	private:

	// added by 7akam
		int socketD;
		int _port;
		vector<struct pollfd> pollfds;

		bool _topicFlag;
		fd_set _masterSet;
		fd_set _readSet;
		map<int, User *> _users;
		map<int, bool> _authenticated;
		map<string, Channel> _channels;
		vector<int> _clients;
		int _maxFd;
		string _password;
		
		void _hundleMessage(string message, int clientFd);
};

std::ostream &			operator<<( std::ostream & o, Server const & i );

#endif /* ********************************************************** SERVER_H */
