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

	if (commandsFromClient["command"] != "QUIT" && (commandsFromClient.find("params") == commandsFromClient.end() || commandsFromClient["params"].empty()))
	{
		server.setBroadcast(ERR_NEEDMOREPARAMS(commandsFromClient["command"]), user.getSocket());
		return;
	}

	// DEBUG //
	std::cout << "[CommandHandler] : " << commandsFromClient["params"] << std::endl;
	//

	executeCommand();

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

void	CommandHandler::executeCommand() {

	// map<string, string>::iterator it = commandsFromClient.begin();

	string cmdStr = commandsFromClient["command"];
	/* DEBUG */
	std::cout << YELLOW << "Executing command: " << RESET << "[" << cmdStr << "]" << std::endl;
	/* ***** */
	e_cmd num = getCMD(cmdStr);
	cmdStr = mapEnumToString[num];

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
		// sendHandshake();
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

	std::map<std::string, Channel *>::iterator it2 = user._channels.begin();
	for ( ; it2 != user._channels.end(); ++it2) {
		it2->second->getUser(oldNick).setNickName(nickname);
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
		// sendHandshake();
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

void	CommandHandler::handleMODE()
{
	std::cout << YELLOW << "MODE command received.." << RESET << std::endl;

	// format  : /MODE #channel flag [param]
	// ou
	// format  : /MODE nickname flag [param]

	// DEBUG //
	std::cout << "handleMODE | commandsFromClient[params] : " << commandsFromClient["params"] << std::endl;
	//
	ModeHandler	mode_handler(commandsFromClient, server, user);
}

void	CommandHandler::handlePING()
{
	std::cout << YELLOW << "PING command received.." << RESET << std::endl;
	user.setPinged(true);
	std::string ping = "PING";
	size_t ping_pos = user.userMessageBuffer.find(ping);
	std::string substr_ping; 
	if (ping_pos != std::string::npos) {
		substr_ping = user.userMessageBuffer.substr(ping_pos);
		substr_ping.replace(0, ping.length(), "PONG");
	}
	std::string reply = user.getPrefix() + " " + substr_ping;
	server.setBroadcast(reply, user.getSocket());
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