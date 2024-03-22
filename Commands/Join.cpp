#include "CommandHandler.hpp"

void	CommandHandler::handleJOIN() {

	std::cout << YELLOW << "JOIN command received.." << RESET << std::endl;

	// format : /join #channel (password)

	std::vector<std::string> params = split(commandsFromClient["params"], " ");
	if (params.begin() + 1 == params.end() || params.begin() + 2 == params.end())
		;
	else
	{
		if (!params.empty())
			server.setBroadcast(ERR_TOOMANYTARGETS(server.hostname, *(params.end() - 1)), user.getSocket());
		return;
	}
	std::string channelName = parse_channelName(*params.begin());
	if (channelName.empty() == true)
	{
		server.setBroadcast(ERR_NOSUCHCHANNEL(server.hostname, user.getNickName(), channelName), user.getSocket());
		return; 
	}
	// check if the channel doesn't exist, creates it
	if (server.channelMap.find(channelName) == server.channelMap.end())
	{
		Channel new_channel(channelName);
		new_channel.setUser(user);
		// set the creator of the channel as operator
		new_channel.setOp(user.getNickName());
		if (params.begin() + 1  != params.end())
			new_channel.setKey(*(params.begin() + 1));
		server.setChannel(new_channel);
		user.setChannel(new_channel);
		server.setBroadcast(MODE_USERMSG(user.getNickName(), "+o"), user.getSocket());
	}
	// if channel already exists
	else
	{
		// handles if mode +k is actived in the channel
		if (server.channelMap[channelName].isInvited(user.getNickName()) == false && server.channelMap[channelName].getProtected() == true)
		{
			if (server.channelMap[channelName].getKey() != *(params.begin() + 1))
			{
				server.setBroadcast(ERR_BADCHANNELKEY(server.hostname, channelName), user.getSocket());
				return;
			}
		}
		// handles if mode +i is actived in the channel
		if (server.channelMap[channelName].getInvit() == true)
		{
			if (server.channelMap[channelName].isInvited(user.getNickName()) == false)
			{
				server.setBroadcast(ERR_INVITEONLYCHAN(server.hostname, user.getNickName() ,channelName), user.getSocket());
				return;
			}
			// delete the user of the invited list as he is able to join
			// so that he cannot join a second time if not invited again
			else
				server.channelMap[channelName].removeInvited(user.getNickName());
		}
	
		if (user._channels.find(channelName) == user._channels.end())
		{
			// handles if mode +l is actived in the channel
			if (server.channelMap[channelName].getLimited() == true)
			{
				std::cout << RED << "getNb() : " << server.channelMap[channelName].getNb() << RESET << std::endl;
				if (server.channelMap[channelName].getNb() == server.channelMap[channelName].getLimit())
				{
					server.setBroadcast(ERR_CHANNELISFULL(server.hostname, user.getNickName(), channelName), user.getSocket());
					return; 
				}
			}
			// add the user
			user.setChannel(server.getChannel(channelName));
			server.channelMap[channelName].setUser(user);
		}
		else
		{
			std::string nickName = user.getNickName();
			server.setBroadcast(ERR_USERONCHANNEL(server.hostname, user.getNickName(), nickName, channelName), user.getSocket());
			return ; 
		}
	}
	// send response to client
	std::string reply = RPL_JOIN(user.getPrefix(), channelName);
	server.setBroadcast(channelName, user.getNickName(), reply);
	std::string topic = server.channelMap[channelName].getTheme();
	if (topic.empty())
		reply += RPL_NOTOPIC(server.hostname, user.getNickName(), channelName) + "\r\n"; //
	else
		reply += RPL_TOPIC(server.hostname, user.getNickName(), channelName, topic);
	server.setBroadcast(reply, user.getSocket());
}