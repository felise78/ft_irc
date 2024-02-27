#include "Channel.hpp"

#include "../User/User.hpp"
#include <iostream>

Channel::Channel(const std::string& name) : _name(name), _nb(0), _limited(false), _topic_restricted(false)
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
	// si le user existe deja
	if (_users[user.getNickName()])
		return ; // throw une erreur ? 
	
	if (_limited == true)
	{
		if (_nb >= _limit)
		{
			std::cout << "User limit has been reached in this channel" << std::endl; // for debug // throw error ? 
			return;
		}
	}
	_users[user.getNickname()] = &user;
	user.setChannel(*this);
	_nb++;
}

void	Channel::setOp(const std::string& nickname)
{
	// Les opérateurs du canal sont généralement désignés par un symbole "@" 
	// devant leur nom d'utilisateur dans la liste des utilisateurs du canal.
	std::string opNickname = "@" + nickname;
	getUser(nickname).setNickName(opNickname);
	_ops.push_back(getUser(opNickname));
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
	return *_users.at(nickname);
}

const std::map<std::string, User*>& 	Channel::getUsers( void ) const
{
	return _users;
}

const std::string& Channel::getOp( const std::string & nickname ) const
{
	std::vector<std::string>::iterator it;

	it = _ops.find(nickname);
	if (it != _ops.end())
		return nickname;
	else
		return NULL;
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
// ------------------- MEMBER FUNCTIONS ---------------------- // 

void	Channel::removeUser(User& user)
{

	std::map<std::string, User*>::iterator it;
	it = _users.find(user.getNickname());
    if (it != _users.end())
	{
		user.removeChannel(*this);
        _users.erase(it);
		_nb--;
	}
}

void	Channel::removeOp(const std::string& opNickname)
{
	// remove the '@' at the beggining of the nickname
	getUser(opNickname).setNickName(opNickname.substr(1));
	// remove op from vector
	_ops.erase(opNickname);
}

void Channel::printUsers( void) const
{
	std::map<std::string, User*>::const_iterator it;

    std::cout << "Users in this channel:" << std::endl;
    for ( it = _users.begin(); it != _users.end(); ++it)
        std::cout << it->second->getNickname() << std::endl;
}