#include "CommandHandler.hpp"

 void	CommandHandler::handleINVITE() {

	std::cout << YELLOW << "INVITE command received.." << RESET << std::endl;

	//format : /INVITE nickname #channel
	
	std::vector<std::string> params = split(commandsFromClient["params"], " ");
	if (params.begin() + 2 != params.end())
	{
		server.setBroadcast(ERR_TOOMANYTARGETS(server.hostname, *(params.end() - 1)), user.getSocket());
		return;
	}
	int nick_fd = server.getFdbyNickName(*params.begin());
	if(nick_fd == -1)
	{
		server.setBroadcast(ERR_NOSUCHNICK(server.hostname, user.getNickName(), *params.begin()), user.getSocket());
		return;
	}
	std::string channelName;
	if (server.channelMap.find(channelName) == server.channelMap.end())
	{
		server.setBroadcast(ERR_NOSUCHCHANNEL(server.hostname, user.getNickName(), channelName), user.getSocket());
		return; 
	}
	server.setBroadcast(RPL_INVITING(server.hostname, user.getNickName(), channelName, *params.begin()), user.getSocket());
	server.setBroadcast(RPL_INVITE(server.hostname, user.getNickName(), *params.begin(), channelName), server.getFdbyNickName(*params.begin()));
}