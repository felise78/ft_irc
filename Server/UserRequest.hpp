#ifndef USERREQUEST_HPP
#define USERREQUEST_HPP

#include <sstream> // std::stringstream
#include <string>
#include <map>
#include <string.h>
#include <iomanip> // std::setw and std::setfill

#include "ServerManager.hpp"

class User;

#define BUF_SIZE 10240 // 100 KB to store the request from the browser

enum requestCMD {

	NONE,
	CAP,
	PASS,
	NICK,
	USER
};

class UserRequest {
	private:

		User 								&_user;

		enum requestCMD						_CMD;
		std::map<std::string, std::string>	_commands;

		// parsing helpers	
		void _basicParsing(const std::string& buffer);
		void _lineParsing(const std::string& line);
		void _authenticatingUser();
		void _extractCMD();

	public:

		// UserRequest(const std::string& requestBuffer); // Constructor reseives the request buffer from HttpServer::_handle() method
		UserRequest(User & user);
		~UserRequest();

		// Getters
		std::string			getCMD();

		// Clean parsing helpers
		std::string			trim(const std::string& str);
		enum requestCMD		isCMD(const std::string& str);
		void				printInHEX(char *buff, int len);

		/* DEBUG */
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
