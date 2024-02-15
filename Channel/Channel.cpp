#include "Channel.hpp"
#include <string>
#include "../User/User.hpp"

Channel::Channel()
{

}

Channel::~Channel()
{

}

void	Channel::setName(const std::string & name)
{
	_name = name;
}

void	Channel::setUser(const User & user)
{
	_users[user.getNickname()] = user; // ou username ?
}

const std::string & Channel::getName( void ) const
{
	return _name;
}

const User& Channel::getUser( const std::string & username )
{
	std::map<std::string, User>::iterator it = _users.find(username);

	if (it == _users.end())
		return it->second; // pas bon il faudra que je change car ce n'est pas correct 
		// faire un throw
	else 
		return it->second;
}