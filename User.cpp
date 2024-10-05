#include "User.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

User::User()
{
}

User::User(int fd, const string &userName, const string &nickName) : _fd(fd), _userName(userName), _nickName(nickName)
{
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

User::~User()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

User &				User::operator=( User const & rhs )
{
	if ( this != &rhs )
	{
		this->_fd = rhs.getFd();
		this->_nickName = rhs.getNickName();
		this->_userName = rhs.getUserName();
		this->_userName = rhs.getIpAdresse();
		this->_operators = rhs.getOperator();
		this->_signed = rhs.getSigned();
		this->_nick = rhs._nick;
		this->_user = rhs._user;
	}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, User const & __unused i )
{
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

int User::getFd() const
{
	return _fd;
}

string User::getUserName() const
{
	return _userName;
}

string User::getNickName() const
{
	return _nickName;
}

string User::getIpAdresse() const
{
	return _ipAdresse;
}

bool User::getOperator() const
{
	return _operators;
}

bool User::getSigned() const
{
	return _signed;
}

bool User::getNick() const
{
	return _nick;
}

bool User::getUser() const
{
	return _user;
}

string User::getRealName() const
{
	return _realName;
}


void User::setFd(int &fd)
{
	_fd = fd;
}

void User::setUserName(const string &userName)
{
	_userName = userName;
}

void User::setNickName(const string &nickName)
{
	_nickName = nickName;
}

void User::setIpAdresse(const string &ipAdresse)
{
	_ipAdresse = ipAdresse;
}

void User::setOperator(bool op)
{
	_operators = op;
}

void User::setSigned(bool signedUser)
{
	_signed = signedUser;
}

void User::setNick(bool nick)
{
	_nick = nick;
}

void User::setUser(bool user)
{
	_user = user;
}

void User::setRealName(const string &realName)
{
	_realName = realName;
}

bool User::getTopicFlag() const
{
	return _topicFlag;
}

void User::setTopicFlag(bool topicFlag)
{
	_topicFlag = topicFlag;
}



// added by me
// int 		User::get_socket()
// {
// 	return (_fd);
// }

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */