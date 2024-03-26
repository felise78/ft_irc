#include "User.hpp"
#include "../Channel/Channel.hpp"
#include <iostream> 

User::User() : 
	_port(0), 
	_fd(0), 
	_hostName(""), 
	_nickName(""), 
	_userName(""), 
	_realName(""),
	_password(""), 
	_status(PASS_NEEDED),
	userMessageBuffer(""),
	responseBuffer(""),
	_authenticated(false), 
	_handshaked(false),
	_pinged(false),
	_cap(false),
	isBot(false),
	skip_mode(true) {
};


User::~User() {}

// --------------------------------------- SETTERS ---------------------------------------- // 

void	User::setStatus(e_status status) { _status = status; }

void	User::setAuthenticated(bool authenticated) { this->_authenticated = authenticated; }

void	User::setHandshaked(bool handshaked) { this->_handshaked = handshaked; }

void	User::setPort(const int& port) { _port = port; }

void	User::setFd(const int& fd) { _fd = fd; }

void 	User::setNickName(const std::string& nickname) { _nickName = nickname; }

void 	User::setUserName(const std::string& username) { _userName = username; }

void 	User::setHostName(const std::string& hostname) { _hostName = hostname; }

void	User::setRealName(const std::string& realname) { _realName = realname; }

void 	User::setPassword(const std::string& password) { _password = password; }

void	User::setChannel(Channel& channel)
{
	if (_channels.find(channel.getName()) != _channels.end()) 			// si le channel est deja dans User
		return;															// dunknow if I still need this protection
	_channels[channel.getName()] = &channel;
}

void	User::setAsBot() { isBot = true; }

void	User::setPinged(bool pinged) { _pinged = pinged; }

void	User::setPrivFds(const int& Fd)
{
	std::vector<int>::iterator it;
	for (it = _privmsg_fds.begin(); it != _privmsg_fds.end(); ++it)
	{
		if (*it == Fd)
			break;
	}
	if (it == _privmsg_fds.end())
		_privmsg_fds.push_back(Fd);
}

// --------------------------------- GETTERS ----------------------------------------- // 

e_status	User::getStatus() { return _status; }

bool	User::authenticated() { return this->_authenticated; }

bool	User::handshaked() { return this->_handshaked; }

bool	User::pinged() { return this->_pinged; }

const int& User::getFd( void ) const { return _fd; }

const std::string& User::getNickName( void ) const { return _nickName; }

const std::string& User::getUserName( void ) const { return _userName; }

const std::string& User::getHostName( void ) const { return _hostName; }

const std::string& User::getRealName() const { return _realName; }

const std::string& User::getPassword( void ) const { return _password; }

Channel& User::getChannel( const std::string& name )
{
	std::map<std::string, Channel*>::iterator it = _channels.find(name);
	return *it->second;
}

// ------------------------------------------------------------------- fonction membres 

std::string	User::getPrefix()
{
	std::stringstream prefix;
	prefix << ":" << _nickName << "!" << _userName;
	if (!_hostName.empty())
		prefix << "@" << _hostName;
	return (prefix.str());
}

void	User::removeChannel(const std::string& channelName)
{
	std::map<std::string, Channel*>::iterator it;
	it = _channels.find(channelName);
    if (it != _channels.end())
        _channels.erase(it);
}