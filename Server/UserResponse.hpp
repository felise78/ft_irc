#ifndef USERRESPONSE_HPP
# define USERRESPONSE_HPP

#include "../Request/server_replies.h"
// #define RPL_WELCOME(nick, host) (":localhost 001 " + nick + " :Welcome to the Internet Relay Network " + nick + "! " + nick + "@" + host + "\r\n")
// #define RPL_YOURHOST(nick) (":localhost 002 " + nick + " :Your host is ircserv, running version ircserv.1\r\n")
// #define RPL_CREATED(nick, date) (":localhost 003 " + nick + " :This server was created " + date + "\r\n")
// #define RPL_MYINFO(nick) (":localhost 004 " + nick + " : ircserv.1 modes itkol\r\n")

#include <string>
#include <sstream>

#include "Server.hpp"
#include "ServerManager.hpp"
class Server;
class User;

class UserResponse {

	private:
		User 			&_user;
		Server const	&_server;

	public:
		UserResponse(User & user, Server const & server);
		~UserResponse();

		void			handshakeResponse();
		void			responseBuilder();
		std::string			getPrefix();

};

#endif