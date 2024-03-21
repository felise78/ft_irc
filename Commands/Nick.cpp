#include "CommandHandler.hpp"

/*
** format : /NICK <nickname>
*/

void	CommandHandler::handleNICK() {

	std::cout << YELLOW << "NICK command received.." << RESET << std::endl;

	std::string nickname = commandsFromClient["params"];
	trim(nickname, " \t\r");
	
	// first check if NICK is valid
	if (nickname.empty()) {
		server.setBroadcast(ERR_NONICKNAMEGIVEN, user.getSocket());
		/* DEBUG */
		std::cout << RED << "[-] " << ERR_NONICKNAMEGIVEN << RESET << std::endl;

		return;
	} // check the length and the characters
	if (nickname.length() > 9 || nickname.length() < 1) {
		server.setBroadcast(ERR_ERRONEUSNICKNAME(nickname), user.getSocket());
		/* DEBUG */
		std::cout << RED << "[-] " << ERR_ERRONEUSNICKNAME(nickname) << RESET << std::endl;

		return;
	}
	// check forbidden characters
	string::const_iterator it;
	for(it = nickname.begin(); it != nickname.end(); ++it) {
		if (std::isalnum(*it) == false) {
			server.setBroadcast(ERR_ERRONEUSNICKNAME(nickname), user.getSocket());
			/* DEBUG */
			std::cout << RED << "[-] " << ERR_ERRONEUSNICKNAME(nickname) << RESET << std::endl;

			return;
		}
	}

	// if the nickname is already in use:
	if (server.getFdbyNickName(commandsFromClient["params"]) != -1) {
		server.setBroadcast(ERR_NICKNAMEINUSE(commandsFromClient["params"]), user.getSocket());
		return;
	}


	// Once all the above passed setting nickname and updating it in all channels
	std::string oldNick = user.getNickName();
	user.setNickName(nickname);

	// si le user a change de nick // qu'il est donc bien REGISTERED // 
	// envoyer un message special a irssi
	// not sure if this should come at the end of the function ?
	if (!oldNick.empty())
		server.setBroadcast(RPL_NICK(oldNick, user.getUserName(), user.getNickName()), user.getSocket()); 

	// segaults cuz we changed for pointers ?
	// std::map<std::string, Channel *>::iterator it2 = user._channels.begin();
	// for ( ; it2 != user._channels.end(); ++it2) {
	// 	it2->second->getUser(oldNick).setNickName(nickname);
	// }

	// the following part is to handle the initial registration of the user
	// if the user is already registered return
	if (user.getStatus() == REGISTERED) {
		// user.responseBuffer = "NICK set to " + nickname + "\r\n";
		return;
	} // if there is no PASS:
	if (user.getStatus() PASS_MATCHED && !user.getUserName().empty()) {
		sendHandshake();
		user.setStatus(REGISTERED);
	}
	// The following logic is not necessary but nice to have anyway !!
	if (user.getStatus() == PASS_NEEDED) {
		server.setBroadcast(ERR_NEEDMOREPARAMS(server.hostname, "PASS"), user.getSocket());
	}
	if (user.getUserName().empty()) {
		server.setBroadcast(ERR_NEEDMOREPARAMS(server.hostname, "USER"), user.getSocket());
	}
}