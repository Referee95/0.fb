#ifndef USER_HPP
# define USER_HPP

# include <iostream>
# include <string>


using std::string;
using std::cout;
using std::cerr;
using std::cin;
using std::endl;

class User
{

	public:

		User();
		User(int fd, const string &userName, const string &nickName);
		User( User const & src );
		~User();

		/// SETTERS AND GETTERS
		string getUserName() const;
		string getNickName() const;
		string getIpAdresse() const;
		bool getOperator() const;
		int getFd() const;
		bool getSigned() const;
		bool getNick() const;
		bool getUser() const;
		bool getTopicFlag() const;
		void setTopicFlag(bool topicFlag);
		string getRealName() const;

		// 
		void setUserName(const string &userName);
		void setNickName(const string &nickName);
		void setIpAdresse(const string &ipAdresse);
		void setOperator(bool op);
		void setFd(int &fd);
		void setSigned(bool signedUser);
		void setNick(bool nick);
		void setUser(bool user);
		void setRealName(const string &realName);


		User &		operator=( User const & rhs );

	private:
		int _fd;
		bool _operators;
		string _userName;
		string _realName;
		string _nickName;
		string _ipAdresse;
		bool _signed;
		bool _nick;
		bool _user;
		bool _topicFlag;
	

};

std::ostream &			operator<<( std::ostream & o, User const & i );

#endif /* ************************************************************ USER_H */