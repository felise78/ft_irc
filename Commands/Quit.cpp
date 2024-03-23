#include "CommandHandler.hpp"

void	CommandHandler::handleQUIT()
{
	std::cout << YELLOW << "QUIT command received.." << RESET << std::endl;

	// format : /QUIT [message]
	
	if (user.getStatus() == DELETED) {
		return;
	}
	// set the user status to DELETED (do not delete here..)
	user.setStatus(DELETED);

	std::string msg = RPL_QUIT(user.getPrefix(), commandsFromClient["params"]);
	
	// if the user is in a channel, remove him from all channels
	// broadcast the QUIT message to all users in the channels
	std::map<std::string, Channel*>::iterator it = user._channels.begin();
	for ( ; it != user._channels.end(); ++it) {

		server.channelMap[it->first].removeUser(user.getNickName());
		server.setBroadcast(it->first, user.getNickName(), msg);
		// deletes the channels of the server if they are empty when User leaves
		if (it->second->getNb() == 0)
			server.channelMap.erase(it->first);
	}
	// sends quit message to all the users the user was talking in privmsg
	std::vector<std::string>::iterator it2 = user._privmsg_nicks.begin();
	for ( ; it2 != user._privmsg_nicks.end(); ++it2) {

		server.setBroadcast(msg, server.getFdbyNickName(*it2));
	}
	server.setBroadcast("", user.getFd());
}