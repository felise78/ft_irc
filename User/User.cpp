#include "User.hpp"
#include "../Channel/Channel.hpp"
#include <iostream> 

User::User() : 
	_port(0), 
	_socket(0), 
	_hostName(""), 
	_nickName(""), 
	_userName(""), 
	_realName(""),
	_password(""), 
	userMessageBuffer(""),
	responseBuffer(""),
	_authenticated(false), 
	_handshaked(false),
	isBot(false) {

};

// User::User(const User& copy)
// {
// 	*this = copy;
// }

// User& User::operator=(const User& src)
// {
// 	if (*this != src)
// 	{
// 		_port = src._port;
// 		_socket = src._socket;
// 		_hostName = src._hostName;
// 		_nickName = src._nickName;
// 		_userName = src._userName;
// 		_realName = src._realName;
// 		_password = src._password;
// 		userMessageBuffer = src.userMessageBuffer;
// 		responseBuffer = src.responseBuffer;
// 		_authenticated = src._authenticated;
// 		_handshaked = src._handshaked;
// 		isBot = src.isBot;
// 		std::map<std::string, Channel*>::const_iterator it;
// 		for(it = src._channels.begin(); it != src._channels.end(); ++it)
// 		{
// 			Channel *new_channel = new Channel(*it->second);
// 			_channels[it->first] = new_channel;
// 		}
// 	}
// 	return *this;
// }

User::~User()
{
	//std::cout << "User " << _userName << " has been destructed" << std::endl;
}

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
	if (_channels.find(channel.getName()) != _channels.end()) // si le channel est deja dans User
		return;
	_channels.insert(std::make_pair(channel.getName(), channel));
}

void	User::setAsBot()
{
	isBot = true;
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

Channel& User::getChannel( const std::string& name )
{
	std::map<std::string, Channel>::iterator it = _channels.find(name);
	return it->second;
}

// fonction membres 

void	User::removeChannel(const std::string& channelName)
{
	std::map<std::string, Channel>::iterator it;
	it = _channels.find(channelName);
    if (it != _channels.end())
        _channels.erase(it);
}

void User::printChannels( void ) const
{
	std::map<std::string, Channel>::const_iterator it;

    std::cout << "Channels in this user:" << std::endl;
    for ( it = _channels.begin(); it != _channels.end(); ++it)
        std::cout << it->second.getName() << std::endl;
}