#include "CommandHandler.hpp"

void	CommandHandler::handleKICK()
{
 	std::cout << YELLOW << "KICK command received.." << RESET << std::endl;

	// format de la commande : /KICK #channel nickname reason

	std::vector<std::string> params = split(commandsFromClient["params"], " ");

	std::string channelName = *params.begin();
	// deja gere par irssi but keeping it for netcat ?
	if (params.begin() + 1 == params.end() || ( params.begin() + 2 == params.end() && channelName[0] == '#'))
	{
		server.setBroadcast(ERR_NEEDMOREPARAMS(server.hostname, commandsFromClient["command"]), user.getFd());
		return;
	}
	if (server.channelMap.find(channelName) == server.channelMap.end())
	{
		server.setBroadcast(ERR_NOSUCHCHANNEL(server.hostname, user.getNickName(), channelName), user.getFd());
		return; 
	}
	if (server.channelMap[channelName]._users.find(user.getNickName()) == server.channelMap[channelName]._users.end())
	{
		server.setBroadcast(ERR_USERNOTINCHANNEL(server.hostname, user.getNickName(), channelName), user.getFd());
		return;
	}
	std::string nickname = *(params.begin() + 1);
	if (server.usersMap.find(server.getFdbyNickName(nickname)) == server.usersMap.end())
	{
		server.setBroadcast(ERR_NOSUCHNICK(server.hostname, user.getNickName(), nickname), user.getFd());
		return;
	}
	if (server.channelMap[channelName].isOp(user.getNickName()) == false)
	{
		server.setBroadcast(ERR_CHANOPRIVSNEEDED(server.hostname, user.getNickName(), channelName), user.getFd());
		return;
	}
	if (server.channelMap[channelName]._users.find(nickname) == server.channelMap[channelName]._users.end())
	{
		server.setBroadcast(ERR_USERNOTINCHANNEL(server.hostname, nickname, channelName), user.getFd());
		return;
	}
	std::string reason;
	for (int i = 2; (params.begin() + i) != params.end() ; ++i)
	{
		std::string word = *(params.begin() + i);
		reason += word;
		if (params.begin() + i + 1 != params.end())
			reason += " ";
	}
	server.channelMap[channelName].removeUser(nickname);
	server.usersMap[server.getFdbyNickName(nickname)].removeChannel(channelName);
	server.setBroadcast(RPL_KICK(user.getPrefix(), channelName, nickname, reason), user.getFd());
	server.setBroadcast(RPL_KICK(user.getPrefix(), channelName, nickname, reason), server.getFdbyNickName(nickname));
	server.setBroadcast(channelName, user.getNickName(), RPL_KICK(user.getPrefix(), channelName, nickname, reason));
}