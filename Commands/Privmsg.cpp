#include "CommandHandler.hpp"

void	CommandHandler::handlePRIVMSG() {

	std::cout << YELLOW << "PRIVMSG command received.." << RESET << std::endl;

	// format : /msg <msgtarget> <message>

	size_t i = commandsFromClient["params"].find_first_of(':');
	if (i == std::string::npos)
	{
		server.setBroadcast(ERR_NOTEXTTOSEND(server.hostname), user.getFd());
		return;
	}
	std::string msgtarget = commandsFromClient["params"].substr(0, i - 1);
	std::string msg = ":" + commandsFromClient["params"].substr(i + 1);
	std::string reply;
	
	// pas sur que ce soit utile car je pense que irssi et meme netcat ne tiennent pas compte des espaces
	// if (msgtarget.find(' ') != std::string::npos)
	// {
	// 	server.setBroadcast(ERR_NOSUCHNICK(server.hostname, user.getNickName(), msgtarget), user.getFd());
	// 	return;
	// }

	// <msgtarget> is a Channel : 
	if (*msgtarget.begin() == '#')
	{
		if (server.channelMap.find(msgtarget) == server.channelMap.end())
		{
			server.setBroadcast(ERR_NOSUCHCHANNEL(server.hostname, user.getNickName(), msgtarget), user.getFd());
			return;
		}
		if (server.channelMap[msgtarget]._users.find(user.getNickName()) == server.channelMap[msgtarget]._users.end())
		{
			server.setBroadcast(ERR_USERNOTINCHANNEL(server.hostname, user.getNickName(), msgtarget), user.getFd());
			return;
		}
		reply = RPL_PRIVMSG(user.getPrefix(), msgtarget, msg);
		server.setBroadcast(msgtarget, user.getNickName(), reply);
	}

	// <msgtarget> is a nickname
	else  
	{
		int nick_fd = server.getFdbyNickName(msgtarget);
		if(nick_fd == -1)
		{
			server.setBroadcast(ERR_NOSUCHNICK(server.hostname, user.getNickName(), msgtarget), user.getFd());
			return;
		}
		reply = RPL_PRIVMSG(user.getPrefix(), msgtarget, msg);
		server.setBroadcast(reply, nick_fd);
	}
}