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
typedef enum requestCMD {

	NONE,
	CAP,
	INFO, //
	INVITE,
	JOIN,
	KICK,
	LIST, //
	MODE,
	NAMES, //
	NICK, 
	NOTICE, //
	OPER,
	PART,
	PASS,
	PING,
	PONG,
	PRIVMSG,
	QUIT, // 
	TOPIC,
	USER,
	VERSION,//
	WHO,//
	WHOIS//

}	e_cmd;

class CommandHandler {

	private:
		void				sendHandshake();

	public:

		ServerManager								&server;
		User										&user;
		map<string, string>							&commandsFromClient;
		map<e_cmd, string>							mapEnumToString; // map to convert CMD enum to string
		map<string, void (CommandHandler::*)() >	cmdToHandler; // map to convert CMD to handler method
		// std::string									channelName;
		// std::string									password;
		// bool										errChannelName;
		std::string									param1;
		std::string									param2;
		CommandHandler(ServerManager& srv, User &usr, map<string, string> &commands);
		~CommandHandler();

		// This method will return enum representation of the string command..
		e_cmd				getCMD(const std::string & str); // enum requestCMD

		void				authenticateUser();
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
	
		//COMMAND UTILS
		const std::string	parse_channelName(std::string& channelName);
};

#endif
