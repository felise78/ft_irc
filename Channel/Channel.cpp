#include "Channel.hpp"
#include "../Request/server_replies.h"

#include "../User/User.hpp"
#include <iostream>

using namespace std;

Channel::Channel(const std::string& name) : _name(name), _theme(""), _key(""), _nb(0), _limit(0), 
_limited(false), _invit_only(false), _topic_restricted(false), _protected(false)
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
	/* DEBUG */
	_users[user.getNickName()] = &user;
	/* ***** */
	// _users[user.getNickName()] = user;
	user.setChannel(*this);
	_nb++;
}

void	Channel::setOp(const std::string& nickname)
{
	// if (_users.find(nickname) == _users.end())
	// 	return;
	// // Les opérateurs du canal sont généralement désignés par un symbole "@" 
	// // devant leur nom d'utilisateur dans la liste des utilisateurs du canal.
	// //std::string opNickname = "@" + nickname;
	// //getUser(nickname).setNickName(opNickname);
	_ops.push_back(nickname);
}

void	Channel::setNb(const int& nb)
{
	_nb = nb;
}

void	Channel::setLimit(const int & limit)
{
	_limit = limit;
	_limited = true;
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

User& Channel::getUser( const std::string & nickname )
{
	// return _users.at(nickname);
	return *_users.at(nickname);
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

const bool&		Channel::getLimited( void ) const
{
	return _limited;
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

bool	Channel::isOp(const std::string& nickname)
{
	vector<string>::iterator it;
	vector<string>:: iterator last = _ops.end();

	for(it = _ops.begin(); it != last; ++it)
	{
		if (*it == nickname)
			return true;
	}
	return false;
}

void	Channel::removeUser(const std::string& nickname)
{
	// std::map<std::string, User>::iterator it;
	std::map<std::string, User* >::iterator it;
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

void	Channel::removeTopic()
{
	_theme = "";
}

void	Channel::removeLimit()
{
	_limited = false;
}

// void	Channel::broadcast(std::string msg)
// {
// 	std::map<std::string, User>::iterator it;

// 	for(it = _users.begin(); it != _users.end(); ++it)
// 	{
// 		server.setBroadcast(msg, it->second.getSocket());
// 	}
	
// }

void Channel::printUsers( void) const
{
	// std::map<std::string, User>::const_iterator it;
	std::map<std::string, User* >::const_iterator it;

    std::cout << "Users in this channel:" << std::endl;
    for ( it = _users.begin(); it != _users.end(); ++it)
        std::cout << it->second->getNickName() << std::endl;
        // std::cout << it->second.getNickName() << std::endl;
}

void Channel::printOps( void) const
{
	std::vector<std::string>::const_iterator it;

    std::cout << "Ops in this channel:" << std::endl;
    for ( it = _ops.begin(); it != _ops.end(); ++it)
        std::cout << *it << std::endl;
}