#ifndef USERREQUEST_HPP
#define USERREQUEST_HPP

#include <sstream> // std::stringstream
#include <string>
#include <map>
#include <string.h>
#include <iomanip> // std::setw and std::setfill
#include <algorithm> // std::transform
#include <cctype> // std::toupper

#include "ServerManager.hpp"
#include "CommandHandler.hpp"

class User;

#define BUF_SIZE 10240 // 100 KB to store the request from the browser

typedef enum requestCMD {

	NONE,
	CAP,
	INFO,
	INVITE,
	JOIN,
	KICK,
	LIST,
	MODE,
	NAMES,
	NICK,
	NOTICE,
	OPER,
	PART,
	PASS,
	PING,
	PONG,
	PRIVMSG,
	QUIT,
	TOPIC,
	USER,
	VERSION,
	WHO,
	WHOIS

}	e_cmd;

class UserRequest {
	private:

		User 								&_user;

		e_cmd 								_CMD; // Current command from the client
		std::map<e_cmd, std::string>		_enumToString; // map to convert enum to string
		std::map<std::string, std::string>	_commandsFromClient; // the key is the command and the value is the rest of the message line

		// parsing helpers	
		void _basicParsing(const std::string& buffer);
		void _lineParsing(const std::string& line);
		void _authenticatingUser();
		void _handleCommand();

	public:

		// UserRequest(const std::string& requestBuffer); // Constructor reseives the request buffer from HttpServer::_handle() method
		UserRequest(User & user);
		~UserRequest();

		// Getters
		std::string			getCMD();

		// Clean parsing helpers
		std::string			trim(const std::string& str);
		enum requestCMD		isCMD(const std::string& str);
		std::string			handleCtrl_D(const std::string& str); // removes Ctrl+D from the string

		/* DEBUG */
		void				printInHEX(char *buff, int len);
		void				printCommands();

};

#endif

/*
** ABOUT `CAP LS` recuest from `irssi` client:
** 

The CAP LS command is used by an IRC client to request a list of capabilities that the server supports. 
The server responds with a list of capabilities it supports. 
The exact capabilities can vary depending on the server software and its configuration.

Here are some common capabilities that might be supported:

`multi-prefix`: Allows multiple prefixes in NAMES list.
`userhost-in-names`: Sends the userhost along with the nick in NAMES list.
`away-notify`: Sends AWAY status change messages to the client.
`account-notify`: Sends account change notifications to the client.
`extended-join`: Sends additional information when a user joins a channel.
`sasl`: Indicates that the server supports SASL authentication.
`tls`: Indicates that the server supports secure connections.
`server-time`: Sends timestamps with server messages.
`batch`: Supports batched messages.
`cap-notify`: Notifies the client when a capability is added or removed.
`echo-message`: Echoes messages back to the client that sent them.
`invite-notify`: Sends notifications when a user is invited to a channel.
`chghost`: Allows changing the displayed username and/or hostname.

The server should respond to the CAP LS command with a CAP * LS : message followed by a space-separated list of capabilities.

For example:
```
CAP * LS :multi-prefix userhost-in-names away-notify account-notify extended-join sasl tls server-time batch cap-notify echo-message invite-notify chghost
```

*/
