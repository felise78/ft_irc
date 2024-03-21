#include "CommandHandler.hpp"

 void	CommandHandler::handleINVITE() {

	std::cout << YELLOW << "INVITE command received.." << RESET << std::endl;

	//format : /INVITE nickname #channel
	
	std::vector<std::string> params = split(commandsFromClient["params"], " ");
	if (params.begin() + 2 != params.end())
	{
		server.setBroadcast(ERR_TOOMANYTARGETS(*(params.end() - 1)), user.getSocket());
		return;
	}
	int nick_fd = server.getFdbyNickName(*params.begin());
	if(nick_fd == -1)
	{
		server.setBroadcast(ERR_NOSUCHNICK(user.getNickName(), *params.begin()), user.getSocket());
		return;
	}
	std::string channelName = parse_channelName(*(params.begin() + 1));
	if (channelName.empty() == true)
	{
		server.setBroadcast(ERR_NOSUCHCHANNEL(user.getNickName(), channelName), user.getSocket());
		return; 
	}

	/* MODIF BY FELISE // it seems we should just send a REPLY and not care about if the chan exists or not
		There is no requirement that the
		channel the target user is being invited to must exist or be a valid
		channel.  To invite a user to a channel which is invite only (MODE
		+i), the client sending the invite must be recognised as being a
		channel operator on the given channel.
	*/

	// creates the channel if it does not exists
	// if(server.channelMap.find(channelName) == server.channelMap.end())
	// {
	// 	Channel new_channel(channelName);
	// 	new_channel.setUser(user);
	// 	new_channel.setOp(user.getNickName());
	// 	new_channel.setUser(server.usersMap[nick_fd]);
	// 	server.setChannel(new_channel);
	// 	server.usersMap[nick_fd].setChannel(new_channel);
	// 	user.setChannel(new_channel);
	// }
	// // if channel already exists
	// else 
	// {
	// 	if (user._channels.find(channelName) == user._channels.end())
	// 	{
	// 		server.setBroadcast(ERR_USERNOTINCHANNEL(user.getNickName(), channelName), user.getSocket());
	// 		return;
	// 	}
	// 	if (server.channelMap[channelName].getLimited() == true)
	// 	{
	// 		if (server.channelMap[channelName].getNb() == server.channelMap[channelName].getLimit())
	// 		{
	// 			server.setBroadcast(ERR_CHANNELISFULL(channelName), user.getSocket());
	// 			return; 
	// 		}
	// 	}
	// 	if (server.channelMap[channelName]._users.find(*params.begin()) != server.channelMap[channelName]._users.end())
	// 	{
	// 		server.setBroadcast(ERR_USERONCHANNEL(*params.begin(), channelName), user.getSocket());
	// 		return ; 
	// 	}
	// 	server.usersMap[nick_fd].setChannel(server.getChannel(channelName));
	// 	server.channelMap[channelName].setUser(server.usersMap[nick_fd]);
	// }
	server.setBroadcast(RPL_INVITING(user.getNickName(), channelName, *params.begin()), user.getSocket());
	server.setBroadcast(RPL_INVITE(user.getNickName(), *params.begin(), channelName), server.getFdbyNickName(*params.begin()));
}