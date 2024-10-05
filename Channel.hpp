#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <string>
# include <vector>
# include "User.hpp"

class Channel
{

	public:

				Channel();
				Channel(const std::string &name);
				Channel( Channel const & src );
				void	addUser( User *user);
				void	removeUser(const User *user);
				void 	setHasPass(bool hasPass);
				void 	setPass(std::string pass);
				void 	setTopic(std::string topic);
				string	getTopic() const;
				string	getPass() const;
				bool	getInviteOnly() const;
				void 	setInviteOnly(bool inviteOnly);
				bool	getHasTopic() const;
				void 	setHasTopic(bool hasTopic);
				std::string getName() const;
				std::vector<User *> getUsers() const;
				void	setUserLimit(int limit);
				int		getUserLimit() const;
				void	addAdmin(User *user);
				void	removeAdmin(User *user);
				std::vector <User *> getAdmin() const;
				void	addInvited(User *user);
				bool	getHasPass() const;

				~Channel();

				Channel &		operator=( Channel const & rhs );

				void	prodcastToChannel(std::string &message, User *client);

			private:
				std::vector<User *> _users;
				std::vector<User *> _invited;
				std::vector<User *> _admins;
				std::string _name;

	 			std::string _modes;
				bool _inviteOnly;
				bool _hasTopic;
				bool _hasPass;
				string _topic;
				string _pass;
				size_t _userLimit;

				

};

std::ostream &			operator<<( std::ostream & o, Channel const & i );

#endif /* ********************************************************* CHANNEL_H */
