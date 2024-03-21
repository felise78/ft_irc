#include "CommandHandler.hpp"

void	CommandHandler::handleKICK()
{
 	std::cout << YELLOW << "KICK command received.." << RESET << std::endl;

	// format de la commande : /KICK #channel nickname

	std::vector<std::string> params = split(commandsFromClient["params"], " ");
	if (params.begin() + 1 != params.end() && params.begin() + 2 != params.end())
	{
		/*DEBUG*/
		int i = 0;
		vector<string>::iterator it = params.begin();
		for ( ; it != params.end(); ++it)
		{
			std::cout << i << std::endl;
			i++;
		}
		server.setBroadcast(ERR_TOOMANYTARGETS(server.hostname, *(params.end() - 1)), user.getSocket());
		// user.responseBuffer = ERR_TOOMANYTARGETS(*(params.end() - 1));
		return;
	}
	else if (params.begin() + 1 == params.end())
	{
		server.setBroadcast(ERR_NEEDMOREPARAMS(server.hostname, commandsFromClient["command"]), user.getSocket());
		// user.responseBuffer = ERR_NEEDMOREPARAMS(commandsFromClient["command"]);
		return;
	}
	std::string channelName = parse_channelName(*params.begin());
	if (channelName.empty() == true || server.channelMap.find(channelName) == server.channelMap.end())
	{
		server.setBroadcast(ERR_NOSUCHCHANNEL(server.hostname, user.getNickName(), channelName), user.getSocket());
		// user.responseBuffer = ERR_NOSUCHCHANNEL(channelName);
		return; 
	}
	if (server.channelMap[channelName]._users.find(user.getNickName()) == server.channelMap[channelName]._users.end())
	{
		server.setBroadcast(ERR_USERNOTINCHANNEL(server.hostname, user.getNickName(), channelName), user.getSocket());
		// user.responseBuffer = ERR_USERNOTINCHANNEL(user.getNickName(), channelName);
		return;
	}
	if (server.channelMap[channelName].isOp(user.getNickName()) == false)
	{
		server.setBroadcast(ERR_CHANOPRIVSNEEDED(server.hostname, channelName), user.getSocket());
		// user.responseBuffer = ERR_CHANOPRIVSNEEDED(channelName);
		return;
	}
	std::string nickname = *(params.begin() + 1);
	if (server.usersMap.find(server.getFdbyNickName(nickname)) == server.usersMap.end())
	{
		server.setBroadcast(ERR_NOSUCHNICK(server.hostname, user.getNickName(), nickname), user.getSocket());
		// user.responseBuffer = ERR_NOSUCHNICK(nickname);
		return;
	}
	if (server.channelMap[channelName]._users.find(nickname) == server.channelMap[channelName]._users.end())
	{
		server.setBroadcast(ERR_USERNOTINCHANNEL(server.hostname, nickname, channelName), user.getSocket());
		// user.responseBuffer = ERR_USERNOTINCHANNEL(nickname, channelName); 
		return;
	}
	server.channelMap[channelName].getUser(nickname).getChannel(channelName).removeUser(nickname);
	server.channelMap[channelName].getUser(nickname).removeChannel(channelName);
	server.channelMap[channelName].removeUser(nickname);
	server.setBroadcast(RPL_KICK(server.hostname, user.getNickName(), channelName, nickname, ""), user.getSocket());
}