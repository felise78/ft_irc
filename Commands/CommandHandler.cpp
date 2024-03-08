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
	if (!user.getNickName().empty() && !user.getUserName().empty()) {
		user.setAuthenticated(true);
		std::cout << "user authenticated " << std::endl;
		// && !user.getPassword().empty() removed PASSWORD condition for now

		// Identifying if te user is a BOT ?!
		if (user.getNickName() == "NeoBot") { // (or any other condition/name to identify the bot)
			std::cout << CYAN << ".. this user is a BOT.. aha !!!" << std::endl;
			user.setAsBot();
		}
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
}

void	CommandHandler::handleCAP() {
	std::cout << YELLOW << "CAP command received.." << RESET << std::endl;
}

void	CommandHandler::handlePASS() {
	std::cout << YELLOW << "PASS command received.." << RESET << std::endl;

	user.setPassword(commandsFromClient["params"]);
}

void	CommandHandler::handleNICK() {
	std::cout << YELLOW << "NICK command received.." << RESET << std::endl;

	std::string nickname = commandsFromClient["params"];
	trim(nickname, " \t\r");
	// parsing nickname;
	if (nickname.length() > 9)
	{
		server.error = 432; // ERR_ERRONEUSNICKNAME
		return; 
	}
	string::const_iterator it;
	for(it = nickname.begin() ; it != nickname.end(); ++it)
	{
		if (std::isalnum(*it) == false)
		{
			server.error = 432; // ERR_ERRONEUSNICKNAME
			return;
		}
	}
	if (server.getFdbyNickName(nickname) != -1)
	{
		server.error = 433; // ERR_NICKNAMEINUSE
		return; 
	}
	user.setNickName(nickname);
}

void	CommandHandler::handleUSER() {
	std::cout << YELLOW << "USER command received.." << RESET << std::endl;

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
			/*DEBUG*/
			std::cout << "Username: " << user.getUserName() << ", HostName: " << user.getHostName() << ", RealName: " << user.getRealName() << std::endl;
			return ;
		}
	}
}

void	CommandHandler::handleJOIN() {

	std::cout << YELLOW << "JOIN command received.." << RESET << std::endl;

	// format : /join #channel (password)

	std::vector<std::string> params = split(commandsFromClient["params"], " ");
	if (params.begin() + 2 != params.end())
	{
		server.error = 407; // ERR_TOOMANYTARGETS
		return;
	}
	std::string channelName = parse_channelName(*params.begin());
	if (channelName.empty() == true)
	{
		server.error = 403; // ERR_NOSUCHCHANNEL
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
	}
	// if channel already exists
	else
	{
		if (server.channelMap[channelName].getInvit() == true)
		{
			server.error = 473; // ERR_INVITEONLYCHAN
			return; 
		}
		if (server.channelMap[channelName].getProtected() == true)
		{
			if (server.channelMap[channelName].getKey() != *(params.begin() + 1))
			{
				server.error = 464 ; // ERR_PASSWDMISMATCH
				return;
			}
		}
		if (user._channels.find(channelName) == user._channels.end())
		{
			if (server.channelMap[channelName].getLimited() == true)
			{
				if (server.channelMap[channelName].getNb() == server.channelMap[channelName].getLimit())
				{
					server.error = 471; // ERR_CHANNELISFULL
					return; 
				}
			}
			// add the user
			user.setChannel(server.getChannel(channelName));
			server.channelMap[channelName].setUser(user);
		}
		else
		{
			server.error = 443; // ERR_USERONCHANNEL
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
		server.error = 412; // ERR_NOTEXTTOSEND
		return;
	}
	std::string msgtarget = commandsFromClient["params"].substr(0, i);
	std::string msg = commandsFromClient["params"].substr(i + 1);
	if (msgtarget.find(' ') != std::string::npos)
	{
		server.error = 401; // ERR_NOSUCHNICK
		return;
	}
	// <msgtarget> is a Channel : 
	if (*msgtarget.begin() == '#')
	{
		if (server.channelMap.find(msgtarget) == server.channelMap.end())
		{
			server.error = 403; // ERR_NOSUCHCHANNEL
			return;
		}
		if (server.channelMap[msgtarget]._users.find(user.getNickName()) == server.channelMap[msgtarget]._users.end())
		{
			server.error = 442; // ERR_NOTONCHANNEL
			return;
		}
		// ! \\ handle envoi du message 
		server.channelMap[msgtarget].broadcast(msg);
		// ! \\ handle envoi du message 
	}
	else  // <msgtarget> is a nickname
	{
		int nick_fd = server.getFdbyNickName(msgtarget);
		if(nick_fd == -1)
		{
			server.error = 401; // ERR_NOSUCHNICK
			return;
		}
		// ! \\ handle envoi du message 
		server.usersMap[nick_fd].userMessageBuffer = msg;
		// ! \\ handle envoi du message 
	}
}

 void	CommandHandler::handleINVITE() {

	std::cout << YELLOW << "INVITE command received.." << RESET << std::endl;

	//format : /INVITE nickname #channel
	
	std::vector<std::string> params = split(commandsFromClient["params"], " ");
	if (params.begin() + 2 != params.end())
	{
		server.error = 407; // ERR_TOOMANYTARGETS
		return;
	}
	int nick_fd = server.getFdbyNickName(*params.begin());
	if(nick_fd == -1)
	{
		server.error = 401; // ERR_NOSUCHNICK
		return;
	}
	std::string channelName = parse_channelName(*(params.begin() + 1));
	if (channelName.empty() == true)
	{
		server.error = 403; // ERR_NOSUCHCHANNEL ??
		return; 
	}
	// creates the channelName if it doesn't exists
	if(server.channelMap.find(channelName) == server.channelMap.end())
	{
		Channel new_channel(channelName);
		new_channel.setUser(user);
		// the user that creates the channel is the op : 
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
			server.error = 442; // ERR_NOTONCHANNEL
			return;
		}
		if (server.channelMap[channelName].getLimited() == true)
		{
			if (server.channelMap[channelName].getNb() == server.channelMap[channelName].getLimit())
			{
				server.error = 471; // ERR_CHANNELISFULL
				return; 
			}
		}
		if (server.channelMap[channelName]._users.find(*params.begin()) != server.channelMap[channelName]._users.end())
		{
			server.error = 443; // ERR_USERONCHANNEL
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
		server.error = 403; // ERR_NOSUCHCHANNEL
		return;
	}
	if (server.channelMap[channelName]._users.find(user.getNickName()) == server.channelMap[channelName]._users.end())
	{
		server.error = 442; // ERR_NOTONCHANNEL
		return;
	}
	if (i == std::string::npos)
	{
		if (server.channelMap[channelName].getTheme().empty() == true)
			server.error = 331; // RPL_NOTOPIC
		else
			server.error = 332; // RPL_TOPIC : server.channelMap[channelName].getTheme();
		return;
	}
	else 	// si il y a un 2e param apr le channel
	{
		std::string topic = commandsFromClient["params"].substr(i + 1);
		if (server.channelMap[channelName].getTopicRestricted() == true)
		{
			if(server.channelMap[channelName].isOp(user.getNickName()) == false)
			{
				server.error = 482; // ERR_CHANOPRIVSNEEDED
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
//  	std::cout << YELLOW << "KICK command received.." << RESET << std::endl;

// 	// format de la commande : /KICK #channel nickname

// 	//std::string channel;
// 	std::string nickname;
	
// 	if (server.channelMap.find(channelName) == server.channelMap.end())
// 	{
// 		server.error = 403; // no such channel
// 		return;
// 	}
// 	if (server.channelMap[channelName].isOp(user.getNickName()) == false)
// 	{
// 		server.error = 482; // chan op privilege is needed
// 		return;
// 	}
// 	if (server.usersMap.find(server.getFdbyNickName(nickname)) == server.usersMap.end())
// 	{
// 		server.error = 401; // no such nickname
// 		return;
// 	}
// 	if (server.channelMap[channelName]._users.find(nickname) == server.channelMap[channelName]._users.end())
// 	{
// 		server.error = 441; // user not in channel
// 		return;
// 	}
// 	server.channelMap[channelName].getUser(nickname).removeChannel(channelName);
// 	server.channelMap[channelName].removeUser(nickname);
// 	server.channelMap[channelName].removeUser(nickname); // dunnow if necessarry to remove it in both
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
