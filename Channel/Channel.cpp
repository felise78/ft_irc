#include "Channel.hpp"

#include "../User/User.hpp"
#include <iostream>

Channel::Channel(const std::string& name) : _name(name), _nb_users(0), _limited(false)
{
	//std::cout << "Channel " << _name << " has been created" << std::endl;
}

Channel::~Channel()
{
	//std::cout << "Channel " << _name << " has been destructed" << std::endl;
}

// ------------------- SETTERS ---------------------- // 

void	Channel::setName(const std::string & name)
{
	_name = name;
}

void	Channel::setTheme(const std::string & theme)
{
	_theme = theme;
}

void	Channel::setKey(const std::string & key)
{
	_key = key;
}

void	Channel::setUser(User& user)
{
	if (_limited == true)
	{
		if (_nb_users >= _limit)
		{
			std::cout << "User limit has been reached in this channel" << std::endl; // for debug // throw error ? 
			return;
		}
	}
	_users[user.getUsername()] = &user;
	user.setChannel(*this);
	_nb_users++;
}

void	Channel::setUser(User& op)
{
	_ops[op.getUsername()] = &op;
}

void	Channel::setNbUsers(const int& nb)
{
	_nb_users = nb;
}

void	Channel::setLimit(const int & limit)
{
	_limit = limit;
}

void	Channel::setInvit(const bool & invit)
{
	_invit_only = invit;
}

// ------------------- GETTERS ---------------------- // 

const std::string& Channel::getName( void ) const
{
	return _name;
}

const std::string& 	Channel::getTheme( void ) const
{
	return _theme;
}

const std::string&	Channel::getKey( void ) const
{
	return _key;
}

User& Channel::getUser( const std::string & username ) const
{
	return *_users.at(username);
}

const std::map<std::string, User*>& 	Channel::getUsers( void ) const
{
	return _users;
}

User& Channel::getOp( const std::string & username ) const
{
	return *_op.at(username);
}

const int& Channel::getNbUsers( void ) const
{
	return _nb_users;
}

const int& 		Channel::getLimit( void ) const
{
	return _limit;
}

const bool& 	Channel::getInvit( void ) const
{
	return _invit_only;
}

// ------------------- MEMBER FUNCTIONS ---------------------- // 

void	Channel::removeUser(User& user)
{

	std::map<std::string, User*>::iterator it;
	it = _users.find(user.getUsername());
    if (it != _users.end())
	{
		user.removeChannel(*this);
        _users.erase(it);
		_nb_users--;
	}
}

void Channel::printUsers( void) const
{
	std::map<std::string, User*>::const_iterator it;

    std::cout << "Users in this channel:" << std::endl;
    for ( it = _users.begin(); it != _users.end(); ++it)
        std::cout << it->second->getUsername() << std::endl;
}