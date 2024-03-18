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
	cmdToHandler["QUIT"] = &CommandHandler::handleQUIT;
	// .. and so on

	// executeCommand();

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
	if (channelName[0] != '#') // ADD && channelName[0] != '&' ?
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
** SUGGEST TO CHANGE THIS LOGIC !!!!
**
** PASS handling, missmatch, etc. TO DO IN `handlePASS`
*/
void	CommandHandler::authenticateUser() {

	// if user is not authenticated, we search for the PASS, NICK and USER commands first
	// if (commandsFromClient["command"] == "CAP") {
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
	if (!user.getNickName().empty() && !user.getUserName().empty() && user.getPassword() == server.getPassword()) {
		user.setAuthenticated(true);
		sendHandshake();
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
** Command Handlers
*/
void	CommandHandler::handleNONE() {
	// do nothing or/and print error message
	std::cout << RED << "[-] command not found.." << RESET << std::endl;
	if (commandsFromClient.find("command") == commandsFromClient.end())
		return ;
	server.setBroadcast(ERR_UNKNOWNCOMMAND(commandsFromClient["command"]), user.getSocket());
}

void	CommandHandler::handleCAP() {
	std::cout << YELLOW << "CAP command received.." << RESET << std::endl;
	// user._cap = true;
}

/*
** format : /PASS <password>
*/
void	CommandHandler::handlePASS() {
	std::cout << YELLOW << "PASS command received.." << RESET << std::endl;

	std::string pass = commandsFromClient["params"];

	// if already registered
	if (user.getStatus() == REGISTERED) {
		server.setBroadcast(ERR_ALREADYREGISTRED, user.getSocket()); //replacing all instances of assigning to user.responseBuffer with setBroadcast server method
		// user.responseBuffer = ERR_ALREADYREGISTRED;
		/* DEBUG */
		std::cout << RED << "[-]" << ERR_ALREADYREGISTRED << RESET << std::endl;
		return;
	}
	// first check is the PASS is not empty
	if (pass.empty() == true) {
		std::string str = "PASS";
		server.setBroadcast(ERR_NEEDMOREPARAMS(str), user.getSocket());
		// user.responseBuffer = ERR_NEEDMOREPARAMS(str);
		/* DEBUG */
		std::cout << RED << "[-] " << ERR_NEEDMOREPARAMS(str) << RESET << std::endl;
		return;
	}
	// pass missmatch check
	if (pass != server.getPassword()) {
		server.setBroadcast(ERR_PASSWDMISMATCH, user.getSocket());
		// user.responseBuffer = ERR_PASSWDMISMATCH;
		/* DEBUG */
		std::cout << RED << "[-] " << ERR_PASSWDMISMATCH << RESET << std::endl;
		return;
	}
	// if the password is correct
	user.setPassword(pass);
	user.setStatus(PASS_MATCHED);
	/* DEBUG */
	// user.responseBuffer = "PASS OK !\r\n";
	std::cout << GREEN << "[+] PASS OK !" << RESET << std::endl;
	/* ***** */

	// if ther is NICK and USER set:
	if (!user.getNickName().empty() && !user.getUserName().empty()) {
		sendHandshake();
		user.setStatus(REGISTERED);
	}
	// if there is NICK and no USER:
	else if (!user.getNickName().empty() && user.getUserName().empty()) {
		std::string str = "USER";
		server.setBroadcast(ERR_NEEDMOREPARAMS(str), user.getSocket());
		// user.responseBuffer = ERR_NEEDMOREPARAMS(str);
	}
	// if there is USER and no NICK:
	else if (user.getNickName().empty() && !user.getUserName().empty()) {
		std::string str = "NICK";
		server.setBroadcast(ERR_NEEDMOREPARAMS(str), user.getSocket());
		// user.responseBuffer = ERR_NEEDMOREPARAMS(str);
	}
}

/*
** format : /NICK <nickname>
*/
void	CommandHandler::handleNICK() {
	std::cout << YELLOW << "NICK command received.." << RESET << std::endl;

	std::string nickname = commandsFromClient["params"];
	trim(nickname, " \t\r");
	
	// first check if NICK is valid
	if (nickname.empty()) {
		server.setBroadcast(ERR_NONICKNAMEGIVEN, user.getSocket());
		// user.responseBuffer = ERR_NONICKNAMEGIVEN;
		/* DEBUG */
		std::cout << RED << "[-] " << ERR_NONICKNAMEGIVEN << RESET << std::endl;

		return;
	} // check the length and the characters
	if (nickname.length() > 9 || nickname.length() < 1) {
		server.setBroadcast(ERR_ERRONEUSNICKNAME(nickname), user.getSocket());
		// user.responseBuffer = ERR_ERRONEUSNICKNAME(nickname);
		/* DEBUG */
		std::cout << RED << "[-] " << ERR_ERRONEUSNICKNAME(nickname) << RESET << std::endl;

		return;
	}
	// check forbidden characters
	string::const_iterator it;
	for(it = nickname.begin(); it != nickname.end(); ++it) {
		if (std::isalnum(*it) == false) {
			server.setBroadcast(ERR_ERRONEUSNICKNAME(nickname), user.getSocket());
			// user.responseBuffer = ERR_ERRONEUSNICKNAME(nickname);
			/* DEBUG */
			std::cout << RED << "[-] " << ERR_ERRONEUSNICKNAME(nickname) << RESET << std::endl;

			return;
		}
	}

	// if the nickname is already in use:
	if (server.getFdbyNickName(commandsFromClient["params"]) != -1) {
		server.setBroadcast(ERR_NICKNAMEINUSE(commandsFromClient["params"]), user.getSocket());
		// user.responseBuffer = ERR_NICKNAMEINUSE(commandsFromClient["params"]);
		return;
	}

	// Once all the above passed setting nickname and updating it in all channels
	std::string oldNick = user.getNickName();
	user.setNickName(nickname);

	std::map<std::string, Channel>::iterator it2 = user._channels.begin();
	for ( ; it2 != user._channels.end(); ++it2) {
		it2->second.getUser(oldNick).setNickName(nickname);
	}

	// the following part is to handle the initial registration of the user
	// if the user is already registered return
	if (user.getStatus() == REGISTERED) {
		// user.responseBuffer = "NICK set to " + nickname + "\r\n";
		return;
	} // if there is no PASS:
	else if (user.getStatus() == PASS_NEEDED) {
		user.setNickName("");
		/* DEBUG */
		// user.responseBuffer = "PASS needed first\r\n";
		std::cout << RED << "[-] PASS needed first" << RESET << std::endl;

		return;
	}
	else if (user.getStatus() == PASS_MATCHED && !user.getUserName().empty()) {
		sendHandshake();
		user.setStatus(REGISTERED);
	}
	// else if (user.getStatus() == PASS_MATCHED && user.getUserName().empty()) {
	// 	// std::string str = "USER";
	// 	// user.responseBuffer = ERR_NEEDMOREPARAMS(str);
	// 	// user.responseBuffer = "NICK is set to " + nickname + ". Also need USER.\r\n";
	// }
}

/*
** format : /USER <username> <hostname> <servername> <realname>
*/
void	CommandHandler::handleUSER() {
	std::cout << YELLOW << "USER command received.." << RESET << std::endl;

	// if there is no PASS:
	if (user.getStatus() == PASS_NEEDED) {
		/* DEBUG */
		// user.responseBuffer = "PASS needed first\r\n";
		std::cout << RED << "[-] PASS needed first" << RESET << std::endl;

		return;
	} // PASS is correct, but no NICK:
	else if (user.getStatus() == PASS_MATCHED && user.getNickName().empty()) {
		std::string str = "NICK";
		/* DEBUG */
		// user.responseBuffer = ERR_NEEDMOREPARAMS(str);
		std::cout << RED << "[-] " << ERR_NEEDMOREPARAMS(str) << RESET << std::endl;
	} // PASS is ok and NICK is set, but no USER:
	else if (user.getStatus() == PASS_MATCHED && !user.getNickName().empty()) {

		std::vector<std::string> params = split(commandsFromClient["params"], " ");

		// check if there are not enough parameters return
		if (!(params.size() == 1 || params.size() >= 4)) {
			std::string str = "USER";
			server.setBroadcast(ERR_NEEDMOREPARAMS(str), user.getSocket());
			// user.responseBuffer = ERR_NEEDMOREPARAMS(str);
			/* DEBUG */
			std::cout << RED << "[-] " << ERR_NEEDMOREPARAMS(str) << RESET << std::endl;
			/* ***** */
			return;
		}
		if (params.size() == 1) {
			user.setUserName(params[0]);
			user.setHostName("localhost");
			user.setRealName("localhost");
		}
		else if (params.size() >= 4) {
			user.setUserName(params[0]);
			user.setHostName(params[1]);
			user.setRealName(params[3]);
		}
		sendHandshake();
		user.setStatus(REGISTERED);
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
		if (!params.empty()) {
			server.setBroadcast(ERR_TOOMANYTARGETS(*(params.end() - 1)), user.getSocket());
			// user.responseBuffer = ERR_TOOMANYTARGETS(*(params.end() - 1));
		}
		return;
	}
	std::string channelName = parse_channelName(*params.begin());
	if (channelName.empty() == true)
	{
		server.setBroadcast(ERR_NOSUCHCHANNEL(channelName), user.getSocket());
		// user.responseBuffer = ERR_NOSUCHCHANNEL(channelName);
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
		// user.responseBuffer = user.getPrefix() + " JOIN " + channelName + "\r\n";
		std::string reply = user.getPrefix() + user.userMessageBuffer;
		std::string topic = server.channelMap[channelName].getTheme();
		if (topic.empty())
			reply += RPL_NOTOPIC(channelName);
		else
			reply += RPL_TOPIC(channelName, topic);
		server.setBroadcast(reply, user.getSocket());
	}
	// if channel already exists
	else
	{
		if (server.channelMap[channelName].getInvit() == true)
		{
			server.setBroadcast(ERR_INVITEONLYCHAN(channelName), user.getSocket());
			// user.responseBuffer = ERR_INVITEONLYCHAN(channelName);
			return; 
		}
		if (server.channelMap[channelName].getProtected() == true)
		{
			if (server.channelMap[channelName].getKey() != *(params.begin() + 1))
			{
				server.setBroadcast(ERR_BADCHANNELKEY(channelName), user.getSocket());
				// user.responseBuffer = ERR_BADCHANNELKEY(channelName);
				return;
			}
		}
		if (user._channels.find(channelName) == user._channels.end())
		{
			if (server.channelMap[channelName].getLimited() == true)
			{
				if (server.channelMap[channelName].getNb() == server.channelMap[channelName].getLimit())
				{
					server.setBroadcast(ERR_CHANNELISFULL(channelName), user.getSocket());
					// user.responseBuffer = ERR_CHANNELISFULL(channelName);
					return; 
				}
			}
			// add the user
			user.setChannel(server.getChannel(channelName));
			server.channelMap[channelName].setUser(user);
			// user.responseBuffer = user.getPrefix() + " JOIN " + channelName + "\r\n";
			std::string reply = user.getPrefix() + user.userMessageBuffer;
			std::string topic = server.channelMap[channelName].getTheme();
			if (topic.empty())
				reply += RPL_NOTOPIC(channelName);
			else
				reply += RPL_TOPIC(channelName, topic);
			server.setBroadcast(reply, user.getSocket());
		}
		else
		{
			std::string nickName = user.getNickName();
			server.setBroadcast(ERR_USERONCHANNEL(nickName, channelName), user.getSocket());
			// user.responseBuffer = ERR_USERONCHANNEL(nickName, channelName);
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
		server.setBroadcast(ERR_NOTEXTTOSEND, user.getSocket());
		// user.responseBuffer = ERR_NOTEXTTOSEND;
		return;
	}
	std::string msgtarget = commandsFromClient["params"].substr(0, i - 1);
	std::string msg = commandsFromClient["params"].substr(i + 1);
	std::string reply;
	if (msgtarget.find(' ') != std::string::npos)
	{
		server.setBroadcast(ERR_NOSUCHNICK(msgtarget), user.getSocket());
		// user.responseBuffer = ERR_NOSUCHNICK(msgtarget);
		return;
	}
	// <msgtarget> is a Channel : 
	if (*msgtarget.begin() == '#')
	{
		if (server.channelMap.find(msgtarget) == server.channelMap.end())
		{
			server.setBroadcast(ERR_NOSUCHCHANNEL(msgtarget), user.getSocket());
			// user.responseBuffer = ERR_NOSUCHCHANNEL(msgtarget);
			return;
		}
		if (server.channelMap[msgtarget]._users.find(user.getNickName()) == server.channelMap[msgtarget]._users.end())
		{
			server.setBroadcast(ERR_USERNOTINCHANNEL(user.getNickName(), msgtarget), user.getSocket());
			// user.responseBuffer = ERR_USERNOTINCHANNEL(user.getNickName(), msgtarget);
			return;
		}
		reply = RPL_PRIVMSG(user.getPrefix(), msgtarget, msg);
		server.setBroadcast(msgtarget, user.getNickName(), reply); // this will add all users fds to the `send_fd_pool` and send the message to all users in the channel
	}
	else  // <msgtarget> is a nickname
	{
		int nick_fd = server.getFdbyNickName(msgtarget);
		if(nick_fd == -1)
		{
			server.setBroadcast(ERR_NOSUCHNICK(msgtarget), user.getSocket());
			// user.responseBuffer = ERR_NOSUCHNICK(msgtarget);
			return;
		}
		// ! \\ handle envoi du message 
		// server.usersMap[nick_fd].userMessageBuffer = msg;
		// ! \\ handle envoi du message
		reply = RPL_PRIVMSG(user.getPrefix(), msgtarget, msg);
		server.setBroadcast(reply, nick_fd); // this will add the fd to the send_fd_pool and send the message to the user
	}
	//NEW from Pia 18/03 17:00 
	// std::string replySender;
	// replySender = user.getPrefix() + user.userMessageBuffer;
	// server.setBroadcast(replySender, user.getSocket());
}

 void	CommandHandler::handleINVITE() {

	std::cout << YELLOW << "INVITE command received.." << RESET << std::endl;

	//format : /INVITE nickname #channel
	
	std::vector<std::string> params = split(commandsFromClient["params"], " ");
	if (params.begin() + 2 != params.end())
	{
		server.setBroadcast(ERR_TOOMANYTARGETS(*(params.end() - 1)), user.getSocket());
		// user.responseBuffer = ERR_TOOMANYTARGETS(*(params.end() - 1));
		return;
	}
	int nick_fd = server.getFdbyNickName(*params.begin());
	if(nick_fd == -1)
	{
		server.setBroadcast(ERR_NOSUCHNICK(*params.begin()), user.getSocket());
		// user.responseBuffer = ERR_NOSUCHNICK(*params.begin());
		return;
	}
	std::string channelName = parse_channelName(*(params.begin() + 1));
	if (channelName.empty() == true)
	{
		server.setBroadcast(ERR_NOSUCHCHANNEL(channelName), user.getSocket());
		// user.responseBuffer = ERR_NOSUCHCHANNEL(channelName);
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
			server.setBroadcast(ERR_USERNOTINCHANNEL(user.getNickName(), channelName), user.getSocket());
			// user.responseBuffer = ERR_USERNOTINCHANNEL(user.getNickName(), channelName);
			return;
		}
		if (server.channelMap[channelName].getLimited() == true)
		{
			if (server.channelMap[channelName].getNb() == server.channelMap[channelName].getLimit())
			{
				server.setBroadcast(ERR_CHANNELISFULL(channelName), user.getSocket());
				// user.responseBuffer = ERR_CHANNELISFULL(channelName);
				return; 
			}
		}
		if (server.channelMap[channelName]._users.find(*params.begin()) != server.channelMap[channelName]._users.end())
		{
			server.setBroadcast(ERR_USERONCHANNEL(*params.begin(), channelName), user.getSocket());
			// user.responseBuffer = ERR_USERONCHANNEL(*params.begin(), channelName);
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
		server.setBroadcast(ERR_NOSUCHCHANNEL(channelName), user.getSocket());
		// user.responseBuffer = ERR_NOSUCHCHANNEL(channelName);
		return;
	}
	if (server.channelMap[channelName]._users.find(user.getNickName()) == server.channelMap[channelName]._users.end())
	{
		server.setBroadcast(ERR_USERNOTINCHANNEL(user.getNickName(), channelName), user.getSocket());
		// user.responseBuffer = ERR_USERNOTINCHANNEL(user.getNickName(), channelName);
		return;
	}
	if (i == std::string::npos)
	{
		if (server.channelMap[channelName].getTheme().empty() == true){
			server.setBroadcast(RPL_NOTOPIC(channelName), user.getSocket());
			// user.responseBuffer = RPL_NOTOPIC(channelName);
		}
		else {
			server.setBroadcast(RPL_TOPIC(channelName, server.channelMap[channelName].getTheme()), user.getSocket());
			// user.responseBuffer = RPL_TOPIC(channelName, server.channelMap[channelName].getTheme());
		}
		return;
	}
	else 	// si il y a un 2e param apr le channel
	{
		std::string topic = commandsFromClient["params"].substr(i + 1);
		if (server.channelMap[channelName].getTopicRestricted() == true)
		{
			if(server.channelMap[channelName].isOp(user.getNickName()) == false)
			{
				server.setBroadcast(ERR_CHANOPRIVSNEEDED(channelName), user.getSocket());
				// user.responseBuffer = ERR_CHANOPRIVSNEEDED(channelName);
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
		server.setBroadcast(ERR_TOOMANYTARGETS(*(params.end() - 1)), user.getSocket());
		// user.responseBuffer = ERR_TOOMANYTARGETS(*(params.end() - 1));
		return;
	}
	else if (params.begin() + 1 == params.end())
	{
		server.setBroadcast(ERR_NEEDMOREPARAMS(commandsFromClient["command"]), user.getSocket());
		// user.responseBuffer = ERR_NEEDMOREPARAMS(commandsFromClient["command"]);
		return;
	}
	std::string channelName = parse_channelName(*params.begin());
	if (channelName.empty() == true || server.channelMap.find(channelName) == server.channelMap.end())
	{
		server.setBroadcast(ERR_NOSUCHCHANNEL(channelName), user.getSocket());
		// user.responseBuffer = ERR_NOSUCHCHANNEL(channelName);
		return; 
	}
	if (server.channelMap[channelName]._users.find(user.getNickName()) == server.channelMap[channelName]._users.end())
	{
		server.setBroadcast(ERR_USERNOTINCHANNEL(user.getNickName(), channelName), user.getSocket());
		// user.responseBuffer = ERR_USERNOTINCHANNEL(user.getNickName(), channelName);
		return;
	}
	if (server.channelMap[channelName].isOp(user.getNickName()) == false)
	{
		server.setBroadcast(ERR_CHANOPRIVSNEEDED(channelName), user.getSocket());
		// user.responseBuffer = ERR_CHANOPRIVSNEEDED(channelName);
		return;
	}
	std::string nickname = *(params.begin() + 1);
	if (server.usersMap.find(server.getFdbyNickName(nickname)) == server.usersMap.end())
	{
		server.setBroadcast(ERR_NOSUCHNICK(nickname), user.getSocket());
		// user.responseBuffer = ERR_NOSUCHNICK(nickname);
		return;
	}
	if (server.channelMap[channelName]._users.find(nickname) == server.channelMap[channelName]._users.end())
	{
		server.setBroadcast(ERR_USERNOTINCHANNEL(nickname, channelName), user.getSocket());
		// user.responseBuffer = ERR_USERNOTINCHANNEL(nickname, channelName); 
		return;
	}
	server.channelMap[channelName].getUser(nickname).getChannel(channelName).removeUser(nickname);
	server.channelMap[channelName].getUser(nickname).removeChannel(channelName);
	server.channelMap[channelName].removeUser(nickname);
	server.setBroadcast(RPL_KICK(user.getNickName(), channelName, nickname, ""), user.getSocket());
	// user.responseBuffer += RPL_KICK(user.getNickName(), channelName, nickname, "");
}

void	CommandHandler::handleMODE()
{
	std::cout << YELLOW << "MODE command received.." << RESET << std::endl;

	try {
		ModeHandler	mode_handler(commandsFromClient, server, user);
	}
	catch (std::exception &e) {
	}
}

void	CommandHandler::handlePING()
{
	std::cout << YELLOW << "PING command received.." << RESET << std::endl;
	user.setPinged(true);
	std::string ping = "PING";
	size_t ping_pos = user.userMessageBuffer.find(ping);
	if (ping_pos != std::string::npos)
		user.userMessageBuffer.replace(ping_pos, ping.length(), "PONG");
	std::string reply = user.getPrefix() + " " + user.userMessageBuffer;
	server.setBroadcast(reply, user.getSocket());
	// user.responseBuffer = user.getPrefix() + " PONG localhost";
}

void	CommandHandler::handlePART()
{
	std::cout << YELLOW << "PART command received.." << RESET << std::endl;
	
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
		server.setBroadcast(ERR_NOSUCHCHANNEL(channelName), user.getSocket());
		// user.responseBuffer = ERR_NOSUCHCHANNEL(channelName);
		return; 
	}
	if (server.channelMap[channelName]._users.find(user.getNickName()) == server.channelMap[channelName]._users.end())
	{
		server.setBroadcast(ERR_USERNOTINCHANNEL(user.getNickName(), channelName), user.getSocket());
		// user.responseBuffer = ERR_USERNOTINCHANNEL(user.getNickName(), channelName); 
		return;
	}
	user._channels[channelName].removeUser(user.getNickName());
	user.removeChannel(channelName);
	server.setBroadcast(RPL_PART(user.getPrefix(), channelName, msg), user.getSocket());
	// user.responseBuffer = RPL_PART(user.getPrefix(), channelName, msg);
	server.setBroadcast(channelName, user.getNickName(), user.responseBuffer);
	server.channelMap[channelName].removeUser(user.getNickName());
}

void	CommandHandler::handleQUIT()
{
	std::cout << YELLOW << "QUIT command received.." << RESET << std::endl;

	// format : /QUIT [message]
	std::string msg;
	if (commandsFromClient["params"].empty() == false) {
		msg = commandsFromClient["params"];
	}
	if (user.getStatus() == DELETED) {
		return;
	}
	// if the user is in a channel, remove him from all channels
	// broadcast the QUIT message to all users in the channels
	std::map<std::string, Channel>::const_iterator it = user._channels.begin();
	for ( ; it != user._channels.end(); ++it) {
		user.removeChannel(it->first);
		server.channelMap[it->first].removeUser(user.getNickName());
		server.setBroadcast(it->first, user.getNickName(), msg);
	}
	// set the user status to DELETED (do not delete here..)
	user.setStatus(DELETED);
}

void	CommandHandler::sendHandshake()
{
	std::string serverCreated = "_server.getCreationDate()";
	std::string hostName = user.getHostName();
	std::string nickName = user.getNickName();

	std::stringstream reply_buffer;
	reply_buffer << RPL_WELCOME(nickName, hostName) << RPL_YOURHOST(nickName)
	<< RPL_CREATED(nickName, serverCreated) << RPL_MYINFO(nickName);
	// user.responseBuffer = reply_buffer.str();
	server.setBroadcast(reply_buffer.str(), user.getSocket());
	user.setHandshaked(true);
}