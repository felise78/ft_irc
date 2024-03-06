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

	// setting the channel if there is one in params
	setChannel();
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

void	CommandHandler::setChannel()
{
	size_t hashPos = commandsFromClient["params"].find('#');
    
    if (hashPos != std::string::npos) {
        size_t spacePos = commandsFromClient["params"].find(' ', hashPos);
		std::string channelName = commandsFromClient["params"].substr(hashPos, spacePos - hashPos);
		if (server.channelMap.find(channelName) == server.channelMap.end())
		{
			server.error = 403; // no such channel
			return; // ou throw ? 
		}
		else 
			_channel = server.channelMap[channelName];
    }
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
		server.error = 432; // erroneus nickname
		return; 
	}
	string::const_iterator it;
	for(it = nickname.begin() ; it != nickname.end(); ++it)
	{
		if (std::isalnum(*it) == false)
		{
			server.error = 432; // erroneus nickname
			return;
		}
	}
	if (server.getFdbyNickName(nickname) != -1)
	{
		server.error = 433; // nickname in use
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
//need to handle the case where the channel name is wrong, eg two ##?
	std::cout << YELLOW << "JOIN command received.." << RESET << std::endl;

	// format : /join #channel (password)
	const std::string channel;
	const std::string password;
	
	if (server.channelMap.find(channel) == server.channelMap.end())
	{
		Channel new_channel(channel);
		server.setChannel(new_channel);
		user.setChannel(new_channel);
		_channel->setUser(user);
		
		// set the creator of the channel as operator
		_channel->setOp(user.getNickName());

		if (password.empty() == false)
			_channel->setKey(password);
	}
	else
	{
		if (server.channelMap[channel]->getInvit() == true)
			return;
		if (server.channelMap[channel]->getProtected() == true)
		{
			if (server.channelMap[channel]->getKey() != password)
				return;
		}
		if (user._channels.find(channel) == user._channels.end())
		{
			user.setChannel(server.getChannel(channel));
			server.getChannel(channel).setUser(user);
		}
		else 
			return ; // une erreur ?
	}

}

void	CommandHandler::handlePRIVMSG() {

	std::cout << YELLOW << "PRIVMSG command received.." << RESET << std::endl;

	// format : /msg <msgtarget> <message>

	std::string msg;
	// <msgtarget> can be a nickname for a private message or the name of a channel for broadcast
	if (_channel) // si le msgtarget est un channel, le channel de CommandHandler est donc set
	{
		if (server.channelMap.find(_channel->getName()) == server.channelMap.end())
		{
			server.error = 403; // no such channel
			return;
		}
		if (_channel->_users.find(user.getNickName()) == _channel->_users.end())
		{
			server.error = 442; // user not in that channel
			return;
		}
		// sinon send le message a tous les Users du channel
		_channel->broadcast(msg);
	}
	// sinon, msgtarget est donc un nickname
	std::string nickname;
	int nick_fd = server.getFdbyNickName(nickname);
	// check if the nickname exists in the server
	if(nick_fd == -1)
	{
		server.error = 401; // no such nickname
		return;
	}
	// send the private message
	server.usersMap[nick_fd].userMessageBuffer = msg;
	
}

void	CommandHandler::handleINVITE() {

	std::cout << YELLOW << "INVITE command received.." << RESET << std::endl;

	// format : /INVITE nickname #channel
	
	std::string channel;
	std::string nickname;

	int nick_fd = server.getFdbyNickName(nickname);
	if(nick_fd == -1)
	{
		server.error = 401; // no such nickname
		return;
	}
	// si le channel n'existe pas , le creer.
	if(server.channelMap.find(channel) == server.channelMap.end())
	{
		Channel new_channel(channel);
		server.setChannel(new_channel);
		user.setChannel(new_channel);
		_channel->setUser(user);
		
		// the user that creates the channel is the op : 
		_channel->setOp(user.getNickName());

		// invite the nickname
		server.usersMap[nick_fd].setChannel(new_channel);
		new_channel.setUser(server.usersMap[nick_fd]);
	}
	else 
	{
		if (user._channels.find(channel) == user._channels.end())
		{
			server.error = 442; // user not in that channel
			return;
		}
		server.usersMap[nick_fd].setChannel(server.getChannel(channel));
		server.getChannel(channel).setUser(server.usersMap[nick_fd]);
	}
}

void	CommandHandler::handleTOPIC()	{

	std::cout << YELLOW << "TOPIC command received.." << RESET << std::endl;

 	// format : /TOPIC #channel 

	std::string topic;	// will be params

	if (server.channelMap.find(_channel->getName()) == server.channelMap.end())
	{
		server.error = 403; // no such channel
		return;
	}
	if (_channel->_users.find(user.getNickName()) == _channel->_users.end())
	{
		server.error = 442; // user not in that channel
		return;
	}
	// si y'a pas de params 
	if (topic.empty() == true) // will be params
	{
		if (_channel->getTheme().empty() == true)
			server.error = 331; // no topic is set
		else
			std::cout << _channel->getTheme() << std::endl; // print juste le topic
			// print ou envoie en privmsg ??
		return;
	}
	// else s' il y a un param apres le nom du channel
	if (_channel->getTopicRestricted() == true)
	{
		if(_channel->isOp(user.getNickName()) == false)
		{
			server.error = 482; // chan op privilege is needed
			return;
		}
	}
	_channel->setTheme(topic);
}

void	CommandHandler::handleKICK()
{
 	std::cout << YELLOW << "KICK command received.." << RESET << std::endl;

	// format de la commande : /KICK #channel nickname

	std::string channel;
	std::string nickname;
	
	if (server.channelMap.find(channel) == server.channelMap.end())
	{
		server.error = 403; // no such channel
		return;
	}
	if (_channel->isOp(user.getNickName()) == false)
	{
		server.error = 482; // chan op privilege is needed
		return;
	}
	if (server.usersMap.find(server.getFdbyNickName(nickname)) == server.usersMap.end())
	{
		server.error = 401; // no such nickname
		return;
	}
	if (server.channelMap[channel]->_users.find(nickname) == server.channelMap[channel]->_users.end())
	{
		server.error = 441; // user not in channel
		return;
	}
	_channel->getUser(nickname).removeChannel(channel);
	_channel->removeUser(nickname);
	server.getChannel(channel).removeUser(nickname); // dunnow if necessarry to remove it in both
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
