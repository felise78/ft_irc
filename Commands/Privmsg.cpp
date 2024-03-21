#include "CommandHandler.hpp"

void	CommandHandler::handlePRIVMSG() {

	std::cout << YELLOW << "PRIVMSG command received.." << RESET << std::endl;

	// format : /msg <msgtarget> <message>

	size_t i = commandsFromClient["params"].find_first_of(':');
	if (i == std::string::npos)
	{
		server.setBroadcast(ERR_NOTEXTTOSEND(server.hostname), user.getSocket());
		return;
	}
	std::string msgtarget = commandsFromClient["params"].substr(0, i - 1);
	std::string msg = ":" + commandsFromClient["params"].substr(i + 1);
	std::string reply;
	if (msgtarget.find(' ') != std::string::npos)
	{
		server.setBroadcast(ERR_NOSUCHNICK(server.hostname, user.getNickName(), msgtarget), user.getSocket());
		return;
	}
	// <msgtarget> is a Channel : 
	if (*msgtarget.begin() == '#')
	{
		if (server.channelMap.find(msgtarget) == server.channelMap.end())
		{
			server.setBroadcast(ERR_NOSUCHCHANNEL(server.hostname, user.getNickName(), msgtarget), user.getSocket());
			return;
		}
		if (server.channelMap[msgtarget]._users.find(user.getNickName()) == server.channelMap[msgtarget]._users.end())
		{
			server.setBroadcast(ERR_USERNOTINCHANNEL(server.hostname, user.getNickName(), msgtarget), user.getSocket());
			return;
		}
		reply = RPL_PRIVMSG(server.hostname, user.getPrefix(), msgtarget, msg);
		server.setBroadcast(msgtarget, user.getNickName(), reply);
	}
	else  // <msgtarget> is a nickname
	{
		int nick_fd = server.getFdbyNickName(msgtarget);
		if(nick_fd == -1)
		{
			server.setBroadcast(ERR_NOSUCHNICK(server.hostname, user.getNickName(), msgtarget), user.getSocket());
			return;
		}
		reply = RPL_PRIVMSG(server.hostname, user.getPrefix(), msgtarget, msg);
		server.setBroadcast(reply, nick_fd);
	}
}