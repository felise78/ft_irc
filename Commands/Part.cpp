#include "CommandHandler.hpp"

void	CommandHandler::handlePART()
{
	std::cout << YELLOW << "PART command received.." << RESET << std::endl;
	
	// format: /PART #channel [message]

	std::string channelName;
	size_t i = commandsFromClient["params"].find_first_of(' ');
	if (i == std::string::npos)
		channelName = commandsFromClient["params"];
	else 
		channelName = commandsFromClient["params"].substr(0, i);
	std::string msg;
	if (i != commandsFromClient["params"].size() && i != std::string::npos)
		msg = commandsFromClient["params"].substr(i + 1, commandsFromClient["params"].size() - i + 1);
	if (server.channelMap.find(channelName) == server.channelMap.end())
	{
		server.setBroadcast(ERR_NOSUCHCHANNEL(server.hostname, user.getNickName(), channelName), user.getSocket());
		return; 
	}
	if (server.channelMap[channelName]._users.find(user.getNickName()) == server.channelMap[channelName]._users.end())
	{
		server.setBroadcast(ERR_USERNOTINCHANNEL(server.hostname, user.getNickName(), channelName), user.getSocket());
		return;
	}
	user.removeChannel(channelName);
	server.setBroadcast(RPL_PART(user.getPrefix(), channelName, msg), user.getSocket());
	server.setBroadcast(channelName, user.getNickName(), user.responseBuffer);
	server.channelMap[channelName].removeUser(user.getNickName());
}