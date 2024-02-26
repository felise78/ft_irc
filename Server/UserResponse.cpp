#include "UserResponse.hpp"

UserResponse::UserResponse(User & user, Server const & server) 
	: _user(user), _server(server) {

	std::cout << CYAN << "\t[UserResponse] constructor called" << RESET << std::endl;
	std::cout << YELLOW << "\tuser.handshaked(): " << _user.handshaked() << RESET << std::endl;

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

	std::string serverName = _server.getServerName();
	std::string serverVersion = "_server.getServerVersion()";
	std::string serverCreated = "_server.getCreationDate()";
	std::string userModes = "<usr_modes>";
	std::string channModes = "<chann_modes>";
	std::string hostName = _user.getHostName();
	std::string nickName = _user.getNickName();
	std::string userName = _user.getUserName();

	std::stringstream reply_buffer;

	reply_buffer << RPL_WELCOME(nickName, nickName, "localhost") << RPL_YOURHOST(nickName.c_str())
	<< RPL_CREATED(nickName.c_str(), serverCreated.c_str()) << RPL_MYINFO(nickName.c_str());
	_user.responseBuffer = reply_buffer.str();
	std::cout << _user.responseBuffer;
	_user.setHandshaked(true);
}

void UserResponse::responseBuilder() {

	// BUILDING RESPONSES BASED ON THE COMMANDS RECEIVED FROM THE CLIENT
	(void) _server; //delete
	if (_user.userMessageBuffer.empty()) {

		_user.responseBuffer = "\t<empty request>\n";
	} 
	else {

		_user.responseBuffer = "\t..coucou. The Matrix has you.. waiting for command..\n";
	}
}

// #include <sstream>
// int	main(void)
// {
// 	std::stringstream strcat;

// 	strcat << RPL_WELCOME("pia", "pia", "localhost") << RPL_YOURHOST("pia") << RPL_CREATED("pia", "1st jan 24") << RPL_MYINFO("pia");
// 	std::cout << strcat.str();
// }