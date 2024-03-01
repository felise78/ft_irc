#include "Channel.hpp"

#include "../User/User.hpp"
#include <iostream>

using namespace std;

Channel::Channel(const std::string& name) : _name(name), _nb(0), _limited(false), _topic_restricted(false), _protected(false)
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
	_protected = true;
}

void	Channel::setUser(User& user)
{
	// si le user existe deja
	if (_users.find(user.getNickName()) != _users.end())
		return ; // throw une erreur ? 
	
	if (_limited == true)
	{
		if (_nb >= _limit)
		{
			std::cout << "User limit has been reached in this channel" << std::endl; // for debug // throw error ? 
			return;
		}
	}
	_users[user.getNickName()] = &user;
	user.setChannel(*this);
	_nb++;
}

void	Channel::setOp(const std::string& nickname)
{
	// protection si le nickname n'existe pas dans le channel
	if (_users.find(nickname) == _users.end())
		return; 

	// Les opérateurs du canal sont généralement désignés par un symbole "@" 
	// devant leur nom d'utilisateur dans la liste des utilisateurs du canal.
	std::string opNickname = "@" + nickname;
	getUser(nickname).setNickName(opNickname);
	_ops.push_back(opNickname);
}

void	Channel::setNb(const int& nb)
{
	_nb = nb;
}

void	Channel::setLimit(const int & limit)
{
	_limit = limit;
}

void	Channel::setInvit(const bool & invit)
{
	_invit_only = invit;
}

void	Channel::setTopicRestricted(const bool& topic)
{
	_topic_restricted = topic;
}

void	Channel::setProtected(const bool& protecd)
{
	_protected = protecd;
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

User& Channel::getUser( const std::string & nickname ) const
{
	map<string, User *>::const_iterator it = _users.find(nickname);
	return *(it->second);
}

const std::string& Channel::getOp( const std::string & nickname ) const
{
	vector<string>::const_iterator it;
	for(it = _ops.begin(); it != _ops.end(); ++it)
	{
		if (*it == nickname)
			break;
	}
	return *it;
}

const int& Channel::getNb( void ) const
{
	return _nb;
}

const int& 		Channel::getLimit( void ) const
{
	return _limit;
}

const bool& 	Channel::getInvit( void ) const
{
	return _invit_only;
}

const bool& Channel::getTopicRestricted() const
{
	return _topic_restricted;
}

const bool& Channel::getProtected() const
{
	return _protected;
}

// ------------------- MEMBER FUNCTIONS ---------------------- // 

void	Channel::removeUser(const std::string nickname)
{

	std::map<std::string, User*>::iterator it;
	it = _users.find(nickname);
    if (it != _users.end())
	{
        _users.erase(it);
		_nb--;
	}
}

void	Channel::removeOp(const std::string& opNickname)
{
	for (std::vector<std::string>::iterator it = _ops.begin(); it != _ops.end(); ++it)
	{
        if (*it == opNickname)
		{
            _ops.erase(it); // Supprime l'op
			getUser(opNickname).setNickName(opNickname.substr(1)); // remove '@'
            break; 
        }
    }
	
	
}

void Channel::printUsers( void) const
{
	std::map<std::string, User*>::const_iterator it;

    std::cout << "Users in this channel:" << std::endl;
    for ( it = _users.begin(); it != _users.end(); ++it)
        std::cout << it->second->getNickName() << std::endl;
}

void Channel::printOps( void) const
{
	std::vector<std::string>::const_iterator it;

    std::cout << "Ops in this channel:" << std::endl;
    for ( it = _ops.begin(); it != _ops.end(); ++it)
        std::cout << *it << std::endl;
}