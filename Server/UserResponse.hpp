#ifndef USERRESPONSE_HPP
# define USERRESPONSE_HPP

/* INITIAL REPLY */
# define RPL_WELCOME(nick) ("Welcome to the Internet Relay Network " + nick ) // 001
# define RPL_YOURHOST(servername, version) ("Your host is " + servername + ", running version " + version ) // 002
# define RPL_CREATED(date) ("This server was created " + date ) // 003
# define RPL_MYINFO(servername, version, usr_modes, chann_modes) (":" + servername + " " + version + " " + usr_modes + " " + chann_modes ) // 004

/* ***** */

#include <string>

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
		void			responceBuilder();

};

#endif
