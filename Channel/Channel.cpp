#include "Channel.hpp"

#include "../User/User.hpp"
#include <iostream>

Channel::Channel(const std::string& name) : _name(name)
{
	std::cout << "Channel " << _name << " has been created" << std::endl;
}

Channel::~Channel()
{
	std::cout << "Channel " << _name << " has been destructed" << std::endl;
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
	//_users.insert(std::make_pair(user.getUsername(), user));
	_users[user.getUsername()] = &user; 
	//si je veux modifier le
	// USER qui a deja cette clef
}

void	Channel::setLimit(const int & limit)
{
	_limit = limit;
}

void	Channel::setInvit(const bool & invit)
{
	_invit = invit;
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
	// std::map<std::string, User>::const_iterator it = _users.find(username);

	// if (it == _users.end())
	// 	return it->second; // pas bon il faudra que je change car ce n'est pas correct 
	// 	// faire un throw
	// else 
	// 	return it->second;
}

const std::map<std::string, User*>& 	Channel::getUsers( void ) const
{
	return _users;
}

const int& 		Channel::getLimit( void ) const
{
	return _limit;
}

const bool& 	Channel::getInvit( void ) const
{
	return _invit;
}

// other member functions

void	Channel::removeUser(User& user)
{
	 auto it = _users.find(user.getUsername()); // Recherche l'utilisateur dans la map
        if (it != _users.end())
		{ // Si l'utilisateur est trouvé
           // delete it->second; // Supprime l'utilisateur de la mémoire heap
            _users.erase(it);
		}
}

void	Channel::printUsers( void ) const
{
	std::map<std::string, User*>::iterator it;

	for (it = _users.begin(); it !=_users.end(); ++it)
	{
		
	}
}