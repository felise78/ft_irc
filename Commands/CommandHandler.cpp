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
	if (channelName.empty())
		;
	else if (channelName[0] != '#')
		channelName = "#" + channelName;
	return channelName;
}


void	CommandHandler::executeCommand() {

	string cmdStr = commandsFromClient["command"];
	e_cmd num = getCMD(cmdStr);

	// update here if enum is modified
	if ((num == 3 || num == 4 || num == 5 || num == 7 || num == 9 || num == 12 || num == 13 || num == 16
	|| num == 18 || num == 19) && (commandsFromClient.find("params") == commandsFromClient.end() || commandsFromClient["params"].empty()))
	{
		server.setBroadcast(ERR_NEEDMOREPARAMS(server.hostname, commandsFromClient["command"]), user.getFd());
		return;
	}
	cmdStr = mapEnumToString[num];
	if (cmdToHandler.find(cmdStr) != cmdToHandler.end())
		(this->*cmdToHandler[cmdStr])();
}

/*
** Command Handlers
*/
void	CommandHandler::handleNONE() {
	std::cout << RED << "[-] command not found.." << RESET << std::endl;
	server.setBroadcast(ERR_UNKNOWNCOMMAND(server.hostname, commandsFromClient["command"]), user.getFd());
}

void	CommandHandler::handleCAP() {
	// std::cout << YELLOW << "CAP command received.." << RESET << std::endl;
	user._cap = true;
}

/*
** format : /PASS <password>
*/
void	CommandHandler::handlePASS() {
	std::cout << YELLOW << "PASS command received.." << RESET << std::endl;

	std::string pass = commandsFromClient["params"];

	// if already registered
	if (user.getStatus() == REGISTERED) {
		server.setBroadcast(ERR_ALREADYREGISTRED(server.hostname), user.getFd());
		return;
	}
	// first check is the PASS is not empty
	if (pass.empty() == true) {
		std::string str = "PASS";
		server.setBroadcast(ERR_NEEDMOREPARAMS(server.hostname, str), user.getFd());
		return;
	}
	// pass missmatch check
	if (pass != server.getPassword()) {
		server.setBroadcast(ERR_PASSWDMISMATCH(server.hostname), user.getFd());
		return;
	}
	// if the password is correct
	user.setPassword(pass);
	user.setStatus(PASS_MATCHED);

	// if there is NICK and USER set:
	if (!user.getNickName().empty() && !user.getUserName().empty()) {
		sendHandshake();
		user.setStatus(REGISTERED);
	}
	
	// The following logic is not necessary but nice to have anyway !!
	if (user.getUserName().empty() && user._cap == false) {
		server.setBroadcast(ERR_NEEDUSERNAME(server.hostname), user.getFd());
	}
	if (user.getNickName().empty() && user._cap == false) {
		server.setBroadcast(ERR_NEEDNICKNAME(server.hostname), user.getFd());
	}
}

/*
** format : /USER <username> <hostname> <servername> <realname>
*/
void	CommandHandler::handleUSER() {
	std::cout << YELLOW << "USER command received.." << RESET << std::endl;

	if (user.getStatus() == PASS_NEEDED || user.getStatus() == PASS_MATCHED) {

		std::vector<std::string> params = split(commandsFromClient["params"], " ");

		// check if there are not enough parameters return
		if (!(params.size() == 1 || params.size() >= 4)) {
			std::string str = "USER";
			server.setBroadcast(ERR_NEEDMOREPARAMS(server.hostname, str), user.getFd());
			return;
		}
		if (params.size() == 1) {
			user.setUserName(params[0]);
			user.setHostName("localhost");
			user.setRealName("localhost");
		}
		else if (params.size() >= 4) {
			user.setUserName(params[0]);
			user.setHostName(params[2]);
			user.setRealName(params[3]);
		}
	}
	if (user.getStatus() == PASS_MATCHED && !user.getNickName().empty()) {
		sendHandshake();
		user.setStatus(REGISTERED);
	}

	// The following logic is not necessary but nice to have anyway !!
	if (user.getNickName().empty() && user._cap == false) {
		server.setBroadcast(ERR_NEEDNICKNAME(server.hostname), user.getFd());
	}
	if (user.getStatus() == PASS_NEEDED && user._cap == false) {
		server.setBroadcast(ERR_NEEDPASSWORD(server.hostname), user.getFd());
	}
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
	std::string ping = "PING";
	size_t ping_pos = user.userMessageBuffer.find(ping);
	std::string substr_ping; 
	if (ping_pos != std::string::npos) {
		substr_ping = user.userMessageBuffer.substr(ping_pos);
		substr_ping.replace(0, ping.length(), "PONG");
	}
	std::string reply = user.getPrefix() + " " + substr_ping;
	server.setBroadcast(reply, user.getFd());
}

void	CommandHandler::sendHandshake()
{
	std::string hostName = user.getHostName();
	std::string nickName = user.getNickName();
	std:: string prefix = user.getPrefix();

	std::stringstream reply_buffer;
	reply_buffer << RPL_WELCOME(server.hostname, nickName, prefix) << RPL_YOURHOST(server.hostname, nickName)
	<< RPL_CREATED(server.hostname, nickName, CREATION_DATE) << RPL_MYINFO(server.hostname, nickName);
	server.setBroadcast(reply_buffer.str(), user.getFd());
	user.setHandshaked(true);
}