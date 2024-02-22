#ifndef COMMANDHANDLER_HPP
#define COMMANDHANDLER_HPP

#include "UserRequest.hpp"

using namespace std;

// Abstract class for the commands
class CommandHandler {

	public:

		User				&_user;
		map<string, string>	&_commandsFromClient;

		ACmd(User &usr, map<string, string> &commands) : { _user = usr; _commandsFromClient = commands; };
		~ACmd() {};

		void	executeCommand();

};

#endif
