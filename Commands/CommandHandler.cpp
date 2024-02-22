#include "CommandHandler.hpp"

CommandHandler::CommandHandler(User &usr, map<string, string> &commands) { 

	_user = usr;
	_commandsFromClient = commands;
}

CommandHandler~CommandHandler() {
}

void	CommandHandler::executeCommand() {

}
