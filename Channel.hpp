#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iostream>
# include <string>
# include <vector>
# include "User.hpp"
#include <algorithm>

class Channel
{

	public:

				Channel();
				Channel(const std::string &name);
				Channel( Channel const & src );
				void addUser( User *user);
				void removeUser(const User *user);

				std::string getName() const;
				std::vector<User *> getUsers() const;

				~Channel();

				Channel &		operator=( Channel const & rhs );

				void	prodcastToChannel(std::string &message, User *client);

			private:
				std::vector<User *> _users;
				std::string _name;
		

};

std::ostream &			operator<<( std::ostream & o, Channel const & i );

#endif /* ********************************************************* CHANNEL_H */
