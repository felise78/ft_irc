#ifndef COMMANDHANDLER_HPP
#define COMMANDHANDLER_HPP

#include "../Channel/Channel.hpp"
#include "../Server/ServerManager.hpp"
#include "../Server/server_utils.hpp"

#include "ModeHandler.hpp"

using namespace std;

class User;
class ServerManager;
class ServerManager;

/*
** The following enumeration represents available commands 
** that the server can handle
*/

//!\\ update the execute_Command() function if enum is modified
//				↓↓↓
typedef enum requestCMD {

	NONE,		// 0
	CAP,		// 1
	INFO, 		// 2
	INVITE,		// 3
	JOIN,		// 4
	KICK,		// 5
	LIST, 		// 6
	MODE,		// 7
	NAMES, 		// 8
	NICK, 		// 9
	NOTICE, 	// 10
	OPER,		// 11
	PART,		// 12
	PASS,		// 13
	PING,		// 14
	PONG,		// 15
	PRIVMSG,	// 16
	QUIT, 	 	// 17
	TOPIC,		// 18
	USER,		// 19
	VERSION,	// 20
	WHO,		// 21
	WHOIS		// 22

}	e_cmd;
// 				↑↑↑
//!\\ update the execute_Command() function if enum is modified

class CommandHandler {

	private:
		void				sendHandshake();

	public:

		ServerManager								&server;
		User										&user;
		map<string, string>							&commandsFromClient;
		map<e_cmd, string>							mapEnumToString; // map to convert CMD enum to string
		map<string, void (CommandHandler::*)() >	cmdToHandler; // map to convert CMD to handler method
		std::string									param1;
		std::string									param2;
		CommandHandler(ServerManager& srv, User &usr, map<string, string> &commands);
		~CommandHandler();

		// This method will return enum representation of the string command..
		e_cmd				getCMD(const std::string & str); // enum requestCMD

		void				executeCommand();

		// COMMAND HANDLERS
		void				handleNONE();
		void				handleCAP();
		void				handlePASS();
		void				handleNICK();
		void				handleUSER();
		void				handlePING();
		void				handleJOIN();
		void				handlePRIVMSG();
		void				handleMODE();
		void				handleKICK();
		void				handleINVITE();
		void				handleTOPIC();
		void				handlePART();
		void				handleQUIT();
	
		//COMMAND UTILS
		const std::string	parse_channelName(std::string& channelName);
		bool				handleemptystring(std::string& channelName);
};

#endif
