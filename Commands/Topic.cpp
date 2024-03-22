#include "CommandHandler.hpp"

void	CommandHandler::handleTOPIC()	{

std::cout << YELLOW << "TOPIC command received.." << RESET << std::endl;

 	// format : /TOPIC #channel [topic]

	size_t i = commandsFromClient["params"].find_first_of(' ');
	std::string channelName = commandsFromClient["params"].substr(0, i);

	// if the channel does not exist
	if (server.channelMap.find(channelName) == server.channelMap.end())
	{
		server.setBroadcast(ERR_NOSUCHCHANNEL(server.hostname, user.getNickName(), channelName), user.getFd());
		return;
	}
	// if the user is not on the channel
	if (server.channelMap[channelName]._users.find(user.getNickName()) == server.channelMap[channelName]._users.end())
	{
		server.setBroadcast(ERR_NOTONCHANNEL(server.hostname, user.getNickName(), channelName), user.getFd());
		//server.setBroadcast(ERR_USERNOTINCHANNEL(server.hostname, user.getNickName(), channelName), user.getFd());
		return;
	}
	// if the user just wants to print the topic
	if (i == std::string::npos)
	{
		if (server.channelMap[channelName].getTheme().empty() == true)
			server.setBroadcast(RPL_NOTOPIC(server.hostname, user.getNickName(), channelName), user.getFd());
		else 
			server.setBroadcast(RPL_TOPIC(server.hostname, user.getNickName(), channelName, server.channelMap[channelName].getTheme()), user.getFd());
		return;
	}
	// if the user wants to change the topic
	else
	{
		std::string topic = commandsFromClient["params"].substr(i + 1);
		
		if (server.channelMap[channelName].getTopicRestricted() == true)
		{
			if(server.channelMap[channelName].isOp(user.getNickName()) == false)
			{
				server.setBroadcast(ERR_CHANOPRIVSNEEDED(server.hostname, channelName), user.getFd());
				return;
			}
		}
		server.channelMap[channelName].setTheme(topic);
		server.setBroadcast(RPL_TOPIC(server.hostname, user.getNickName(), channelName, topic), user.getFd());
		server.setBroadcast(channelName, user.getNickName(), RPL_TOPIC(server.hostname, user.getPrefix(), channelName, topic));
	}
}