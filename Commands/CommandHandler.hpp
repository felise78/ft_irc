#ifndef COMMANDHANDLER_HPP
#define COMMANDHANDLER_HPP

#include "../Request/UserRequestParsing.hpp"
#include "../Channel/Channel.hpp"

using namespace std;

class User;

/*
** The following enumeration represents available commands 
** that the server can handle
*/
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


class CommandHandler {

	public:

		ServerManager					&server;
		User							&user;
		map<string, string>				&commandsFromClient;
		map<e_cmd, string>				mapEnumToString; // map to convert CMD enum to string
		map<string, void (CommandHandler::*)() >	cmdToHandler; // map to convert CMD to handler method
		Channel*							_channel; // pointer to channel, if channel concerned. pointer so it can be NULL.
		CommandHandler(User &usr, map<string, string> &commands);
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
		void				handleJOIN();
		void				handlePRIVMSG();
		void				handleMODE();
		void				handleKICK();
		void				handleINVITE();
		void				handleTOPIC();
};

#endif