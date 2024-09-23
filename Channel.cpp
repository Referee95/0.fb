# include "Channel.hpp"
# include <sys/socket.h>



/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Channel::Channel()
{
}

Channel::Channel(const std::string &name) : _name(name)
{

}

Channel::Channel( const Channel & src )
{
	*this = src;
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Channel::~Channel()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Channel &				Channel::operator=( Channel const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Channel const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/


void Channel::prodcastToChannel(std::string &message, User *client)
{	
	cout << "salamaa\n";
	for (std::vector<User *>::iterator it = _users.begin(); it != _users.end(); it++)
	{
		User *user = *it;
		if (user != client)
			send(user->getFd(), message.c_str(), message.size(), 0);
	}
	
}

void Channel::addUser( User *user)
{
	_users.push_back(user);
}

void Channel::removeUser(const User *user)
{
	_users.erase(std::remove(_users.begin(), _users.end(), user), _users.end());
}

std::string Channel::getName() const
{
	return _name;
}

std::vector<User *> Channel::getUsers() const
{
	return _users;
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */