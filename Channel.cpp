# include "Channel.hpp"
# include <sys/socket.h>



/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Channel::Channel()
{
	this->setUserLimit(0);
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
	if ( this != &rhs )
	{
		this->_users = rhs.getUsers();
		this->_invited = rhs._invited;
		this->_admins = rhs._admins;
		this->_name = rhs.getName();
		this->_inviteOnly = rhs.getInviteOnly();
		this->_hasTopic = rhs.getHasTopic();
		this->_hasPass = rhs.getHasPass();
		this->_topic = rhs.getTopic();
		this->_pass = rhs.getPass();
		this->_userLimit = rhs.getUserLimit();
	}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Channel const & __unused i )
{
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

void	Channel::setInviteOnly(bool inviteOnly)
{
	_inviteOnly = inviteOnly;
}

void	Channel::setHasTopic(bool hasTopic)
{
	_hasTopic = hasTopic;
}
void Channel::setHasPass(bool hasPass)
{
	_hasPass = hasPass;
}
void Channel::setPass(std::string pass)
{
	_pass = pass;
}
void Channel::setTopic(std::string topic)
{
	_topic = topic;
}
std::string Channel::getTopic() const
{
	return _topic;
}
std::string Channel::getPass() const
{
	return _pass;
}

void Channel::setUserLimit(int limit)
{
	_userLimit = limit;
}

int Channel::getUserLimit() const
{
	return _userLimit;
}

void Channel::addAdmin(User *user)
{
	_admins.push_back(user);
}

void Channel::removeAdmin(User *user)
{
	_admins.erase(std::remove(_admins.begin(), _admins.end(), user), _admins.end());
}

std::vector <User *> Channel::getAdmin() const
{
	return _admins;
}

void Channel::addInvited(User *user)
{
	_invited.push_back(user);
}

bool Channel::getInviteOnly() const
{
	return _inviteOnly;
}

bool Channel::getHasTopic() const
{
	return _hasTopic;
}

bool Channel::getHasPass() const
{
	return _hasPass;
}

/* ************************************************************************** */