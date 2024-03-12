#include "CommandHandler.hpp"
#include "../User/User.hpp"

CommandHandler::CommandHandler(ServerManager& srv, User &usr, map<string, string> &commands) :
	server(srv), user(usr), commandsFromClient(commands) { 

	// Filling in the `mapEnumToString` map to convert enum to string, this will help to identify the command and execute when needed
	mapEnumToString[NONE] = "NONE";
	mapEnumToString[CAP] = "CAP";
	mapEnumToString[INFO] = "INFO";
	mapEnumToString[INVITE] = "INVITE";
	mapEnumToString[JOIN] = "JOIN";
	mapEnumToString[KICK] = "KICK";
	mapEnumToString[LIST] = "LIST";
	mapEnumToString[MODE] = "MODE";
	mapEnumToString[NAMES] = "NAMES";
	mapEnumToString[NICK] = "NICK";
	mapEnumToString[NOTICE] = "NOTICE";
	mapEnumToString[OPER] = "OPER";
	mapEnumToString[PART] = "PART";
	mapEnumToString[PASS] = "PASS";
	mapEnumToString[PING] = "PING";
	mapEnumToString[PONG] = "PONG";
	mapEnumToString[PRIVMSG] = "PRIVMSG";
	mapEnumToString[QUIT] = "QUIT";
	mapEnumToString[TOPIC] = "TOPIC";
	mapEnumToString[USER] = "USER";
	mapEnumToString[VERSION] = "VERSION";
	mapEnumToString[WHO] = "WHO";
	mapEnumToString[WHOIS] = "WHOIS";

	// Filling in the `cmdToHandler` map execute command given the command string
	cmdToHandler["NONE"] = &CommandHandler::handleNONE;
	cmdToHandler["CAP"] = &CommandHandler::handleCAP;
	cmdToHandler["PASS"] = &CommandHandler::handlePASS;
	cmdToHandler["NICK"] = &CommandHandler::handleNICK;
	cmdToHandler["USER"] = &CommandHandler::handleUSER;
	cmdToHandler["JOIN"] = &CommandHandler::handleJOIN;
	cmdToHandler["PRIVMSG"] = &CommandHandler::handlePRIVMSG;
	cmdToHandler["TOPIC"] = &CommandHandler::handleTOPIC;
	cmdToHandler["INVITE"] = &CommandHandler::handleINVITE;
	cmdToHandler["KICK"] = &CommandHandler::handleKICK;
	cmdToHandler["MODE"] = &CommandHandler::handleMODE;
	cmdToHandler["PING"] = &CommandHandler::handlePING;
	cmdToHandler["PART"] = &CommandHandler::handlePART;
	// .. and so on


	if (user.authenticated()) {
		// process the commands directly
		executeCommand();
	}
	else {
		// if user is not authenticated, we search for the PASS, NICK and USER commands first
		authenticateUser();
	}
}

CommandHandler::~CommandHandler() {
}

e_cmd	CommandHandler::getCMD(const std::string & str) {

	std::map<e_cmd, std::string>::iterator it = mapEnumToString.begin();

	for (; it != mapEnumToString.end(); it++) {
		if (it->second == str) {
			return it->first;
		}
	}
	return NONE;
}

const std::string	CommandHandler::parse_channelName(std::string& channelName)
{
	if (channelName[0] != '#')
		return "";
	string::iterator it = channelName.begin() + 1;
	for ( ; it != channelName.end(); ++it)
	{
		if (std::isalnum(*it) == false)
			return "";
	}
	return channelName;
}

/*
** This function verifies if user is authenticated by checking if the user has sent NICK, USER and PASS 
** set in `User` class. If so, the user is authenticated and can send other Commands to the server.
**
** This logic might be changed .. TO DISCUSS !!
*/
void	CommandHandler::authenticateUser() {

	// if user is not authenticated, we search for the PASS, NICK and USER commands first
	// if (commandsFromClient.find("CAP") != commandsFromClient.end()) {
	// 	handleCAP(); // this one might not be needed
	// }
	// if (commandsFromClient.find("NICK") != commandsFromClient.end()) {
	if (commandsFromClient["command"] == "NICK") {
		handleNICK();
	}
	// if (commandsFromClient.find("PASS") != commandsFromClient.end()) {
	if (commandsFromClient["command"] == "PASS") {
		handlePASS();
	}
	// if (commandsFromClient.find("USER") != commandsFromClient.end()) {
	if (commandsFromClient["command"] == "USER") {
		handleUSER();
	}
	// setting authenticated to true if the user has sent NICK, USER and PASS
	// NEED to add the PASS check as well !!!
	if (!user.getNickName().empty() && !user.getUserName().empty() && !user.getPassword().empty()) {
		user.setAuthenticated(true);
		// Identifying if te user is a BOT ?!
		if (user.getNickName() == "NeoBot") { // (or any other condition/name to identify the bot)
			std::cout << CYAN << ".. this user is a BOT.. aha !!!" << std::endl;
			user.setAsBot();
		}
	}
	else
	{
		user.responseBuffer += "\r\n";
		user.responseBuffer += ERR_NOTREGISTERED;
	}
}

void	CommandHandler::executeCommand() {

	// map<string, string>::iterator it = commandsFromClient.begin();

	string cmdStr = commandsFromClient["command"];
	/* DEBUG */
	std::cout << YELLOW << "Executing command: " << RESET << "[" << cmdStr << "]" << std::endl;
	/* ***** */

	// The synax is important here !! (`cmdToHandler[cmdStr]()` - won't work)
	// first we get the pointer to the handler method and then we call it on `this` object
	if (cmdToHandler.find(cmdStr) != cmdToHandler.end())
		(this->*cmdToHandler[cmdStr])();
}

/*
** Command Hanlers
*/
void	CommandHandler::handleNONE() {
	// do nothing or/and print error message
	std::cout << RED << "[-] command not found.." << RESET << std::endl;
	if (commandsFromClient.find("command") == commandsFromClient.end())
		return ;
	user.responseBuffer = ":localhost 421 ";
	user.responseBuffer += commandsFromClient["command"];
	user.responseBuffer += " :Unknown command";
	// std::string command = commandsFromClient["command"];
	// user.responseBuffer = ERR_UNKNOWNCOMMAND(command); //Doesn't compile? Mystery
}

void	CommandHandler::handleCAP() {
	std::cout << YELLOW << "CAP command received.." << RESET << std::endl;
}

void	CommandHandler::handlePASS() {
	std::cout << YELLOW << "PASS command received.." << RESET << std::endl;

	if (commandsFromClient.find("params") == commandsFromClient.end())
	{
		std::string pass = "PASS";
		user.responseBuffer = ERR_NEEDMOREPARAMS(pass);
		return;
	}
	if (!(user.getPassword().empty()))
	{
		user.responseBuffer = ERR_ALREADYREGISTRED;
		return ;
	}
	std::string pass = commandsFromClient["params"];
	if (pass == server.getPassword())
		user.setPassword(commandsFromClient["params"]);
	else
		user.responseBuffer = ERR_PASSWDMISMATCH;
}

void	CommandHandler::handleNICK() {
	std::cout << YELLOW << "NICK command received.." << RESET << std::endl;

	std::string nickname = commandsFromClient["params"];
	trim(nickname, " \t\r");
	// parsing nickname;
	if (nickname.length() > 9)
	{
		user.responseBuffer = ERR_ERRONEUSNICKNAME(nickname);
		return; 
	}
	string::const_iterator it;
	for(it = nickname.begin() ; it != nickname.end(); ++it)
	{
		if (std::isalnum(*it) == false)
		{
			user.responseBuffer = ERR_ERRONEUSNICKNAME(nickname);
			return;
		}
	}
	if (server.getFdbyNickName(nickname) != -1)
	{
		user.responseBuffer = ERR_NICKNAMEINUSE(nickname);
		return; 
	}
	std::map<std::string, Channel>::iterator it2 = user._channels.begin();
	for( ; it2 != user._channels.end(); ++it2)
		it2->second.getUser(user.getNickName()).setNickName(nickname);
	user.setNickName(nickname);
}

void	CommandHandler::handleUSER() {
	std::cout << YELLOW << "USER command received.." << RESET << std::endl;

	if (!(user.getUserName().empty()))
	{
		user.responseBuffer = ERR_ALREADYREGISTRED;
		return ;
	}
	std::vector<std::string> params = split(commandsFromClient["params"], " ");
	vector<string>::iterator hostnameIt = params.begin() + 1;
	vector<string>::iterator realnameIt = params.begin() + 3;
	for (vector<string>::iterator it = params.begin(); it != params.end(); it++)
	{
		if (it == params.begin()) {
			user.setUserName(*it);
		}
		if (it == hostnameIt) {
			string hostname = "localhost";
			user.setHostName(hostname);
		}
		if (it == realnameIt)
		{
			string realName = *it;
			if (realName[0] == ':')
			{
				realName = (realName).substr(1);
				it++;
				for (; it != params.end(); it++)
				{
					realName += " ";
					realName += (*it);
				}
			}
			user.setRealName(realName);
			return ;
		}
	}
}

void	CommandHandler::handleJOIN() {

	std::cout << YELLOW << "JOIN command received.." << RESET << std::endl;

	// format : /join #channel (password)

	std::vector<std::string> params = split(commandsFromClient["params"], " ");
	if (params.begin() + 1 == params.end() || params.begin() + 2 == params.end())
		;
	else
	{
		server.error = 407;
		if (params.begin() != params.end())
			user.responseBuffer = ERR_TOOMANYTARGETS(*(params.end() - 1));
		return;
	}
	std::string channelName = parse_channelName(*params.begin());
	if (channelName.empty() == true)
	{
		user.responseBuffer = ERR_NOSUCHCHANNEL(channelName);
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
		std::string topic = server.channelMap[channelName].getTheme();
		user.responseBuffer = user.getPrefix() + " JOIN " + channelName + "\r\n";
		user.responseBuffer += RPL_TOPIC(channelName, topic);
	}
	// if channel already exists
	else
	{
		if (server.channelMap[channelName].getInvit() == true)
		{
			user.responseBuffer = ERR_INVITEONLYCHAN(channelName);
			return; 
		}
		if (server.channelMap[channelName].getProtected() == true)
		{
			if (server.channelMap[channelName].getKey() != *(params.begin() + 1))
			{
				user.responseBuffer = ERR_BADCHANNELKEY(channelName);
				return;
			}
		}
		if (user._channels.find(channelName) == user._channels.end())
		{
			if (server.channelMap[channelName].getLimited() == true)
			{
				if (server.channelMap[channelName].getNb() == server.channelMap[channelName].getLimit())
				{
					user.responseBuffer = ERR_CHANNELISFULL(channelName);
					return; 
				}
			}
			// add the user
			user.setChannel(server.getChannel(channelName));
			server.channelMap[channelName].setUser(user);
			std::string topic = server.channelMap[channelName].getTheme();
			user.responseBuffer = user.getPrefix() + " JOIN " + channelName + "\r\n";
			user.responseBuffer += RPL_TOPIC(channelName, topic);
		}
		else
		{
			std::string nickName = user.getNickName();
			user.responseBuffer = ERR_USERONCHANNEL(nickName, channelName);
			return ; 
		}
	}

}

void	CommandHandler::handlePRIVMSG() {

	std::cout << YELLOW << "PRIVMSG command received.." << RESET << std::endl;

	// format : /msg <msgtarget> <message>

	size_t i = commandsFromClient["params"].find_first_of(':');
	if (i == std::string::npos)
	{
		user.responseBuffer = ERR_NOTEXTTOSEND;
		return;
	}
	std::string msgtarget = commandsFromClient["params"].substr(0, i - 1);
	std::string msg = commandsFromClient["params"].substr(i + 1);
	std::string reply;
	if (msgtarget.find(' ') != std::string::npos)
	{
		user.responseBuffer = ERR_NOSUCHNICK(msgtarget);
		return;
	}
	// <msgtarget> is a Channel : 
	if (*msgtarget.begin() == '#')
	{
		if (server.channelMap.find(msgtarget) == server.channelMap.end())
		{
			user.responseBuffer = ERR_NOSUCHCHANNEL(msgtarget);
			return;
		}
		if (server.channelMap[msgtarget]._users.find(user.getNickName()) == server.channelMap[msgtarget]._users.end())
		{
			user.responseBuffer = ERR_USERNOTINCHANNEL(user.getNickName(), msgtarget);
			return;
		}
		reply = RPL_PRIVMSG(user.getPrefix(), msgtarget, msg);
		server.setBroadcast(reply); // this will add all users fds to the `send_fd_pool` and send the message to all users in the channel
	}
	else  // <msgtarget> is a nickname
	{
		int nick_fd = server.getFdbyNickName(msgtarget);
		if(nick_fd == -1)
		{
			user.responseBuffer = ERR_NOSUCHNICK(msgtarget);
			return;
		}
		// ! \\ handle envoi du message 
		// server.usersMap[nick_fd].userMessageBuffer = msg;
		// ! \\ handle envoi du message
		reply = RPL_PRIVMSG(user.getPrefix(), msgtarget, msg);
		server.setBroadcast(reply, nick_fd); // this will add the fd to the send_fd_pool and send the message to the user
	}
}

 void	CommandHandler::handleINVITE() {

	std::cout << YELLOW << "INVITE command received.." << RESET << std::endl;

	//format : /INVITE nickname #channel
	
	std::vector<std::string> params = split(commandsFromClient["params"], " ");
	if (params.begin() + 2 != params.end())
	{
		user.responseBuffer = ERR_TOOMANYTARGETS(*(params.end() - 1));
		return;
	}
	int nick_fd = server.getFdbyNickName(*params.begin());
	if(nick_fd == -1)
	{
		user.responseBuffer = ERR_NOSUCHNICK(*params.begin());
		return;
	}
	std::string channelName = parse_channelName(*(params.begin() + 1));
	if (channelName.empty() == true)
	{
		user.responseBuffer = ERR_NOSUCHCHANNEL(channelName);
		return; 
	}
	// creates the channelName if it doesn't exists
	if(server.channelMap.find(channelName) == server.channelMap.end())
	{
		Channel new_channel(channelName);
		new_channel.setUser(user);
		new_channel.setOp(user.getNickName());
		new_channel.setUser(server.usersMap[nick_fd]);
		server.setChannel(new_channel);
		server.usersMap[nick_fd].setChannel(new_channel);
		user.setChannel(new_channel);
	}
	else // channel already exists
	{
		if (user._channels.find(channelName) == user._channels.end())
		{
			user.responseBuffer = ERR_USERNOTINCHANNEL(user.getNickName(), channelName);
			return;
		}
		if (server.channelMap[channelName].getLimited() == true)
		{
			if (server.channelMap[channelName].getNb() == server.channelMap[channelName].getLimit())
			{
				user.responseBuffer = ERR_CHANNELISFULL(channelName);
				return; 
			}
		}
		if (server.channelMap[channelName]._users.find(*params.begin()) != server.channelMap[channelName]._users.end())
		{
			user.responseBuffer = ERR_USERONCHANNEL(*params.begin(), channelName);
			return ; 
		}
		server.usersMap[nick_fd].setChannel(server.getChannel(channelName));
		server.channelMap[channelName].setUser(server.usersMap[nick_fd]);
	}
}

void	CommandHandler::handleTOPIC()	{

std::cout << YELLOW << "TOPIC command received.." << RESET << std::endl;

 	// format : /TOPIC #channel [topic]

	size_t i = commandsFromClient["params"].find_first_of(' ');
	std::string channelName = commandsFromClient["params"].substr(0, i);
	if (server.channelMap.find(channelName) == server.channelMap.end())
	{
		user.responseBuffer = ERR_NOSUCHCHANNEL(channelName);
		return;
	}
	if (server.channelMap[channelName]._users.find(user.getNickName()) == server.channelMap[channelName]._users.end())
	{
		user.responseBuffer = ERR_USERNOTINCHANNEL(user.getNickName(), channelName);
		return;
	}
	if (i == std::string::npos)
	{
		if (server.channelMap[channelName].getTheme().empty() == true)
			user.responseBuffer = RPL_NOTOPIC(channelName);
		else
			user.responseBuffer = RPL_TOPIC(channelName, server.channelMap[channelName].getTheme());
		return;
	}
	else 	// si il y a un 2e param apr le channel
	{
		std::string topic = commandsFromClient["params"].substr(i + 1);
		if (server.channelMap[channelName].getTopicRestricted() == true)
		{
			if(server.channelMap[channelName].isOp(user.getNickName()) == false)
			{
				user.responseBuffer = ERR_CHANOPRIVSNEEDED(channelName);
				return;
			}
		}
		if (topic.empty())
			server.channelMap[channelName].removeTopic();
		// et pour le cas ou topic est une string d'espaces ?
		server.channelMap[channelName].setTheme(topic);  
	}
}

void	CommandHandler::handleKICK()
{
 	std::cout << YELLOW << "KICK command received.." << RESET << std::endl;

	// format de la commande : /KICK #channel nickname

	std::vector<std::string> params = split(commandsFromClient["params"], " ");
	if (params.begin() + 2 != params.end())
	{
		user.responseBuffer = ERR_TOOMANYTARGETS(*(params.end() - 1));
		return;
	}
	else if (params.begin() + 1 == params.end())
	{
		user.responseBuffer = ERR_NEEDMOREPARAMS(commandsFromClient["command"]);
		return;
	}
	std::string channelName = parse_channelName(*params.begin());
	if (channelName.empty() == true || server.channelMap.find(channelName) == server.channelMap.end())
	{
		user.responseBuffer = ERR_NOSUCHCHANNEL(channelName);
		return; 
	}
	if (server.channelMap[channelName]._users.find(user.getNickName()) == server.channelMap[channelName]._users.end())
	{
		user.responseBuffer = ERR_USERNOTINCHANNEL(user.getNickName(), channelName);
		return;
	}
	if (server.channelMap[channelName].isOp(user.getNickName()) == false)
	{
		user.responseBuffer = ERR_CHANOPRIVSNEEDED(channelName);
		return;
	}
	std::string nickname = *(params.begin() + 1);
	if (server.usersMap.find(server.getFdbyNickName(nickname)) == server.usersMap.end())
	{
		user.responseBuffer = ERR_NOSUCHNICK(nickname);
		return;
	}
	if (server.channelMap[channelName]._users.find(nickname) == server.channelMap[channelName]._users.end())
	{
		user.responseBuffer = ERR_USERNOTINCHANNEL(nickname, channelName); 
		return;
	}
	server.channelMap[channelName].getUser(nickname).removeChannel(channelName);
	server.channelMap[channelName].removeUser(nickname);
	server.channelMap[channelName].removeUser(nickname);
	user.responseBuffer = RPL_KICK(user.getNickName(), channelName, nickname, "");
}

void	CommandHandler::handleMODE()
{
	std::cout << YELLOW << "MODE command received.." << RESET << std::endl;
	
	ModeHandler	mode_handler(commandsFromClient, server, user);
}

void	CommandHandler::handlePING()
{
	std::cout << YELLOW << "PING command received.." << RESET << std::endl;
	user.setPinged(true);
}

void	CommandHandler::handlePART()
{
	std::cout << YELLOW << "PING command received.." << RESET << std::endl;
	
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
		user.responseBuffer = ERR_NOSUCHCHANNEL(channelName);
		return; 
	}
	if (server.channelMap[channelName]._users.find(user.getNickName()) == server.channelMap[channelName]._users.end())
	{
		user.responseBuffer = ERR_USERNOTINCHANNEL(user.getNickName(), channelName); 
		return;
	}
	if (msg.empty() == false)
		handlePRIVMSG();
	user._channels[channelName].removeUser(user.getNickName());
	user.removeChannel(channelName);
	if (msg.empty())
		user.responseBuffer = user.getPrefix() + " PART " + channelName + "\r\n";
}