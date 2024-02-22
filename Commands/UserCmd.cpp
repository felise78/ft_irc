#include "UserCmd.hpp"


void	UserCmd::executeCommand() {

	// Setting up `User` name and `User` real name
	_user.setUserName(_commandsFromClient["USER"]);	
}
