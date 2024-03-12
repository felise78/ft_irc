#include "UserResponse.hpp"

UserResponse::UserResponse(User & user, Server const & server) 
	: _user(user), _server(server) {

	std::cout << CYAN << "\t[UserResponse] constructor called" << RESET << std::endl;
	std::cout << YELLOW << "\tuser.handshaked(): " << std::boolalpha << _user.handshaked() << RESET << std::endl;

	(void) _server;
	if (_user.handshaked() == false) {
		handshakeResponse();
	}
	else {
		responseBuilder();
	}
}

UserResponse::~UserResponse() {
}

void UserResponse::handshakeResponse() {

	// std::string serverName = _server.getServerName();
	if (_user.authenticated() == false)
		return ;
	// std::string serverVersion = "_server.getServerVersion()";
	// std::string serverCreated = "_server.getCreationDate()";
	// std::string hostName = _user.getHostName();
	// std::string nickName = _user.getNickName();
	// std::string userName = _user.getUserName();
	// std::string serverName = _server.getServerName();

	// std::stringstream reply_buffer;
	// reply_buffer << RPL_WELCOME(nickName, hostName) << RPL_YOURHOST(nickName)
	// << RPL_CREATED(nickName, serverCreated) << RPL_MYINFO(nickName);
	// _user.responseBuffer = reply_buffer.str();
	_user.setHandshaked(true);
}

void UserResponse::responseBuilder() {

	// BUILDING RESPONSES BASED ON THE COMMANDS RECEIVED FROM THE CLIENT
		_user.responseBuffer += "\r\n";
		std::cout << "for response buffer: \"" << _user.responseBuffer << "\", message is: \"" << _user.userMessageBuffer << "\"" << std::endl;
}

std::string	UserResponse::getPrefix()
{
	std::stringstream prefix;
	prefix << ":" << _user.getNickName() << "!" << _user.getUserName();
	std::string hostName = _user.getHostName();
	if (!hostName.empty())
		prefix << "@" << hostName;
	return (prefix.str());
}
