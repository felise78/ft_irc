#ifndef USERRESPONSE_HPP
# define USERRESPONSE_HPP

// /* INITIAL REPLY */
// # define RPL_WELCOME(nick) ("001 Welcome to the Internet Relay Network " + nick ) // 001
// # define RPL_YOURHOST(servername, version) ("Your host is " + servername + ", running version " + version ) // 002
// # define RPL_CREATED(date) ("This server was created " + date ) // 003
// # define RPL_MYINFO(servername, version, usr_modes, chann_modes) (":" + servername + " " + version + " " + usr_modes + " " + chann_modes ) // 004

// /* ***** */

#define RPL_WELCOME(nick, host) (":localhost 001 " + nick + " :Welcome to the Internet Relay Network " + nick + "! " + nick + "@" + host + "\r\n")
#define RPL_YOURHOST(nick) (":localhost 002 " + nick + " :Your host is ircserv, running version ircserv.1\r\n")
#define RPL_CREATED(nick, date) (":localhost 003 " + nick + " :This server was created " + date + "\r\n")
#define RPL_MYINFO(nick) (":localhost 004 " + nick + " : ircserv.1 modes itkol\r\n")

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

};

#endif

/*
*/
