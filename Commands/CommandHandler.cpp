#include "CommandHandler.hpp"
#include "../User/User.hpp"

CommandHandler::CommandHandler(User &usr, map<string, string> &commands) :
	user(usr), commandsFromClient(commands) { 

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


/*
** This function verifies if user is authenticated by checking if the user has sent NICK, USER and PASS 
** set in `User` class. If so, the user is authenticated and can send other Commands to the server.
**
** This lohic might be changed .. TO DISCUSS !!
*/
void	CommandHandler::authenticateUser() {

	// if user is not authenticated, we search for the PASS, NICK and USER commands first
	if (commandsFromClient.find("CAP") != commandsFromClient.end()) {
		handleCAP(); // this one might not be needed
	}
	if (commandsFromClient.find("NICK") != commandsFromClient.end()) {
		handleNICK();
	}
	if (commandsFromClient.find("PASS") != commandsFromClient.end()) {
		handlePASS();
	}
	if (commandsFromClient.find("USER") != commandsFromClient.end()) {
		handleUSER();
	}

	// setting authenticated to true if the user has sent NICK, USER and PASS
	// NEED to add the PASS check as well !!!
	if (!user.getNickName().empty() && !user.getUserName().empty() && !user.getPassword().empty()) {
		user.setAuthenticated(true);
	}
}

void	CommandHandler::executeCommand() {

	map<string, string>::iterator it = commandsFromClient.begin();

	for (; it != commandsFromClient.end(); it++) {

		e_cmd cmd = getCMD(it->first);
		string cmdStr = mapEnumToString[cmd]; // if the command is not found, the string will be "NONE" which will map to the `handleNONE` method

		/* DEBUG */
		std::cout << YELLOW << "Executing command: " << RESET << "[" << cmdStr << "]" << std::endl;
		/* ***** */

		// The synax is important here !! (`cmdToHandler[cmdStr]()` - won't work)
		// first we get the pointer to the handler method and then we call it on `this` object
		(this->*cmdToHandler[cmdStr])();
	}
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

	user.setPassword(commandsFromClient["PASS"]);
}

void	CommandHandler::handleNICK() {
	std::cout << YELLOW << "NICK command received.." << RESET << std::endl;

	user.setNickName(commandsFromClient["NICK"]);
}

void	CommandHandler::handleUSER() {
	std::cout << YELLOW << "USER command received.." << RESET << std::endl;

	user.setUserName(commandsFromClient["USER"]);
}

void	CommandHandler::handleINVITE() {

	std::cout << YELLOW << "INVITE command received.." << RESET << std::endl;

	// format de la commade : /INVITE nickname #channel
}

// void	CommandHandler::handleTOPIC()	{

// 	std::cout << YELLOW << "TOPIC command received.." << RESET << std::endl;

// 	// format de la commande : /TOPIC #channel 


// 	std::cout << channel.getTheme() << std::endl;

// 	// if param apres le nom du channel
// 	if (user.getCanModifyTopic() == true)
// 	{
// 		channel.setTheme(new_theme);
// 	}
// }

// void	CommandHandler::handleKICK()
// {
// 	std::cout << YELLOW << "KICK command received.." << RESET << std::endl;

// 	if (user.getIsOp() == false)
// 		return;

// 	// format de la commande : /KICK #channel nickname
// }

// void	CommandHandler::handleMODE()
// {
// 	std::cout << YELLOW << "MODE command received.." << RESET << std::endl;

// 	if (user.getIsOp() == false)
// 		return;
	
// 	//format de la commande :  /mode #channel flag
// 	std::string channel;
// 	std::string flag;

// 	if(flag == "-i")								
// 	{
// 		if (user.getChannel(channel))
// 		if (channel.getInvit() == true) 
// 			channel.setInvit(false);
// 	}
// 	else if(flag  == "+i") 					
// 	{
// 		if (channel.getInvit() == false)
// 			channel.setInvit(true);
// 	}
// 	else if(flag == "-t")					
// 	{
// 		if (user.getCanModifyTopic() == true)
// 				user.setCanModifyTopic(false);
// 	}
// 	else if(flag == "+t")						
// 	{
// 		if (user.getCanModifyTopic() == false)
// 				user.setCanModifyTopic(true);
// 	}
// 	else if(flag == "-k")							
// 	{
// 		// retirer le mot de passe du channel
// 	}
// 	else if(flag == "+k")
// 	{
// 		// definir un mot de passe				// /mode #channel +k password
// 	}
// 	else if(flag == "-o")
// 		;										// /mode #channel -o user
// 	else if(flag == "+o")
// 		;										// /mode #channel +o user
// 	else if(flag == "-l")
// 		;										// /mode #channel -l
// 	else if(flag == "+l")
// 		;										// /mode #channel +l int
// 	// else
// 	// 	; // error unknown flag or parsed before ?
// }