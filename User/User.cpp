#include "User.hpp"
#include "../Channel/Channel.hpp"

#include <iostream> 

User::User(const std::string & nickname, const std::string & username) : _nickname(nickname), _username(username)
{
	std::cout << "User " << _username << " has been created" << std::endl;
}

User::~User()
{
	std::cout << "User " << _username << " has been destructed" << std::endl;
}

// ------------------- SETTERS ---------------------- // 

void	User::setPort(const int& port)
{
	_port = port;
}

void	User::setSocket(const int& socket)
{
	_socket = socket;
}

void 	User::setNickname(const std::string& nickname)
{
	_nickname = nickname;
}

void 	User::setUsername(const std::string& username)
{
	_username = username;
}

void 	User::setHostname(const std::string& hostname)
{
	_hostname = hostname;
}

void 	User::setPassword(const std::string& password)
{
	_password = password;
}

void	User::setChannel(Channel& channel)
{
	//_channels.insert(std::make_pair(channel.getName(), channel));
	_channels[channel.getName()] = &channel;
	// checker si le channel existe deja et voir quel comportement je veux
}

void	User::setIsOp( const bool& isOp )
{
	_isOp = isOp;
}

// ------------------- GETTERS ---------------------- // 

const int& User::getPort( void ) const
{
	return _port;
}

const int& User::getSocket( void ) const
{
	return _socket;
}

const std::string& User::getNickname( void ) const
{
	return _nickname;
}

const std::string& User::getUsername( void ) const
{
	return _username;
}

const std::string& User::getHostname( void ) const
{
	return _hostname;
}

const std::string& User::getPassword( void ) const
{
	return _password;
}

Channel& User::getChannel( const std::string& name ) const
{
	return *_channels.at(name);
}

const std::map<std::string, Channel*>& User::getChannels( void ) const
{
	return _channels;
} 

const bool& User::isOp( void ) const
{
	return _isOp;
}

// ------------------- MEMBER FUNCTIONS ---------------------- // 

void	User::removeChannel(Channel& channel)
{
	std::map<std::string, Channel*>::iterator it;
	it = _channels.find(channel.getName());
    if (it != _channels.end())
	{
		channel.removeUser(*this);
        _channels.erase(it);
	}
}

void User::printChannels( void ) const
{
	std::map<std::string, Channel*>::const_iterator it;

    std::cout << "Channels in this user:" << std::endl;
    for ( it = _channels.begin(); it != _channels.end(); ++it)
        std::cout << it->second->getName() << std::endl;
}