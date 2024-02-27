#include "User.hpp"
#include "../Channel/Channel.hpp"
#include <iostream> 

// User::User(const int& fd) : _socket(fd)
// {
// 	//std::cout << "User has been created" << std::endl;
// }

// User::~User()
// {
// 	//std::cout << "User " << _userName << " has been destructed" << std::endl;
// }

// --------------------------------------- SETTERS ---------------------------------------- // 

void	User::setAuthenticated(bool authenticated)
{ this->_authenticated = authenticated; }

void	User::setHandshaked(bool handshaked) { this->_handshaked = handshaked; }

void	User::setPort(const int& port)
{
	_port = port;
}

void	User::setSocket(const int& socket)
{
	_socket = socket;
}

void 	User::setNickName(const std::string& nickname)
{
	_nickName = nickname;
}

void 	User::setUserName(const std::string& username)
{
	_userName = username;
}

void 	User::setHostName(const std::string& hostname)
{
	_hostName = hostname;
}

void 	User::setPassword(const std::string& password)
{
	_password = password;
}

void	User::setChannel(Channel& channel)
{
	if (_channels.find(channel) != _channels.end()) // si le channel est deja dans User
		return;
	_channels[channel.getName()] = &channel;
}

// ---------------------------------------- GETTERS ----------------------------------------- // 

bool	User::authenticated() { return this->_authenticated; }

bool	User::handshaked() { return this->_handshaked; }

const int& User::getPort( void ) const
{
	return _port;
}

const int& User::getSocket( void ) const
{
	return _socket;
}

const std::string& User::getNickName( void ) const
{
	return _nickName;
}

const std::string& User::getUserName( void ) const
{
	return _userName;
}

const std::string& User::getHostName( void ) const
{
	return _hostName;
}

const std::string& User::getPassword( void ) const
{
	return _password;
}

Channel& User::getChannel( const std::string& name ) const
{
	if (_channels[name] == _channels.end())
		return NULL;
	else
		return _channels[name];
}

// fonction membres 

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