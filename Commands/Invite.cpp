#include "CommandHandler.hpp"

void	CommandHandler::handleINVITE() {

	std::cout << YELLOW << "INVITE command received.." << RESET << std::endl;

	//format : /INVITE nickname #channel
	
	std::vector<std::string> params = split(commandsFromClient["params"], " ");
	if (params.begin() + 2 != params.end())
	{
		server.setBroadcast(ERR_TOOMANYTARGETS(server.hostname, *(params.end() - 1)), user.getFd());
		return;
	}
	std::string channelName = parse_channelName(*(params.begin() + 1));
	if (channelName.empty() || server.channelMap.find(channelName) == server.channelMap.end())
	{
		server.setBroadcast(ERR_NOSUCHCHANNEL(server.hostname, user.getNickName(), *(params.begin() + 1)), user.getFd());
		return; 
	}
	int nick_fd = server.getFdbyNickName(*params.begin());
	if (nick_fd == -1)
	{
		server.setBroadcast(ERR_NOSUCHNICK(server.hostname, user.getNickName(), *params.begin()), user.getFd());
		return;
	}
	if (user._channels.find(channelName) == user._channels.end())
	{	
		server.setBroadcast(ERR_NOTONCHANNEL(server.hostname, user.getNickName(), channelName), user.getFd());
		return ;
	}
	if (server.usersMap[nick_fd]._channels.find(channelName) != server.usersMap[nick_fd]._channels.end())
	{
		server.setBroadcast(ERR_USERONCHANNEL(server.hostname, user.getNickName(), *params.begin(), channelName), user.getFd());
		return;
	}
	server.setBroadcast(RPL_INVITING(server.hostname, user.getNickName(), channelName, *params.begin()), user.getFd());
	server.setBroadcast(RPL_INVITE(user.getPrefix(), *params.begin(), channelName), server.getFdbyNickName(*params.begin()));
	server.channelMap[channelName].setInvited(*params.begin());
}