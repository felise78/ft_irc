#include "UserRequest.hpp"

UserRequest::UserRequest(User & user) 
	: _user(user), _CMD(NONE) {

	_enumToString[NONE] = "NONE";
	_enumToString[CAP] = "CAP";
	_enumToString[INFO] = "INFO";
	_enumToString[INVITE] = "INVITE";
	_enumToString[JOIN] = "JOIN";
	_enumToString[KICK] = "KICK";
	_enumToString[LIST] = "LIST";
	_enumToString[MODE] = "MODE";
	_enumToString[NAMES] = "NAMES";
	_enumToString[NICK] = "NICK";
	_enumToString[NOTICE] = "NOTICE";
	_enumToString[OPER] = "OPER";
	_enumToString[PART] = "PART";
	_enumToString[PASS] = "PASS";
	_enumToString[PING] = "PING";
	_enumToString[PONG] = "PONG";
	_enumToString[PRIVMSG] = "PRIVMSG";
	_enumToString[QUIT] = "QUIT";
	_enumToString[TOPIC] = "TOPIC";
	_enumToString[USER] = "USER";
	_enumToString[VERSION] = "VERSION";
	_enumToString[WHO] = "WHO";
	_enumToString[WHOIS] = "WHOIS";

	/* DEBUG 
	std::cout << CYAN << "\tuserMessageBuffer: " << RESET << std::endl;
	std::cout << MAGENTA << _user.userMessageBuffer << RESET;
	std::cout << CYAN << "\tuserMessageBuffer in HEX: ('\\r' character in hex is [0d] and '\\n' is [0a])" << RESET << std::endl;
	std::cout << MAGENTA;
	printInHEX((char *)_user.userMessageBuffer.c_str(), _user.userMessageBuffer.length());
	std::cout << RESET;
	*/
	/* ***** */

	_basicParsing(_user.userMessageBuffer); // filling in the `_commandsFromClient` map here

	_handleCommand(); // extracting the commands one by one and calling respective command handlers..

	_authenticatingUser();
}

UserRequest::~UserRequest() {
}

void	UserRequest::_basicParsing(const std::string& buffer) {

	std::istringstream	ss(buffer);
	std::string			line;

	while (std::getline(ss, line, '\n')) {

		line = trim(line);

		_lineParsing(line);
	}

	// This will remove the `\r\n` from the end of the buffer..
	_user.userMessageBuffer = trim(_user.userMessageBuffer);
}

/*
** Here we save each line of the request in the _commandsFromClient map as a key-value pair
** The `key` is the first word of the line and the `value` is the rest of the line
** The Command/key is converted to upper case and `/` is removed if it exists
*/
void	UserRequest::_lineParsing(const std::string& line) {
	//std::cout << CYAN << "in parseLine().." << RESET << std::endl;

	std::istringstream	ss(line);
	std::string			firstWord;
	std::string			theRest;

	std::getline(ss, firstWord, ' ');
	std::getline(ss, theRest);

	firstWord = handleCtrl_D(firstWord); // removing Ctrl+D and `^D` from the command string
	
	firstWord = trim(firstWord);
	theRest = trim(theRest);

	// Removing the first `/` if it exists
	if (!firstWord.empty() && firstWord[0] == '/') {
		firstWord.erase(0, 1);
	}

	// Converting the first word to upper case
	std::transform(firstWord.begin(), firstWord.end(), firstWord.begin(), ::toupper);

	if (firstWord.length() != 0 || theRest.length() != 0) {
		// _commandsFromClient.insert(std::make_pair(firstWord, theRest));
		_commandsFromClient[firstWord] = theRest;
	}

}

/*
** This function checks if the user is authenticated by checking if the user has sent NICK, USER and PASS 
** set in `User` class. If so, the user is authenticated and can send messages to the server.
**
** This lohic might be changed .. TO DISCUSS !!
*/
void	UserRequest::_authenticatingUser() {

	/* DEBUG 
	// This logic shall be done in the specific command handlers (NICK, USER, PASS..)
	if (_commandsFromClient.find("NICK") != _commandsFromClient.end()) {
		_user.setNickName(_commandsFromClient["NICK"]);
	}
	if (_commandsFromClient.find("USER") != _commandsFromClient.end()) {
		_user.setUserName(_commandsFromClient["USER"]);
	}
	if (_commandsFromClient.find("PASS") != _commandsFromClient.end()) {
		_user.setPassword(_commandsFromClient["PASS"]);
	}
	*/

	// if (!_user.getNickName().empty() && !_user.getUserName().empty()) {
	if (!_user.getNickName().empty() && !_user.getUserName().empty() && !_user.getPassword().empty()) {
		_user.setAuthenticated(true);
	}
}

/*
** using typedef on the map iterator to make it easier to read
**
** Here we iterate through the _commandsFromClient map and extract/identify the first word of the line as a COMMAND
*/
void	UserRequest::_handleCommand() {

	CommandHandler		commandHandler(_user, _commandsFromClient);
}

// Getters
std::string	UserRequest::getCMD() {
	
	std::map<e_cmd, std::string>::iterator it = _enumToString.find(_CMD);

	return it->second;
}

/*
** Clean parsing helpers.
**
** `trim()` removes unnecessary spaces and new line characters etc.
*/
std::string	UserRequest::trim(const std::string& str) {

	std::string	trimmed;

	trimmed = str;

	trimmed.erase(trimmed.find_last_not_of('\r') + 1);
	trimmed.erase(trimmed.find_last_not_of('\n') + 1);
	trimmed.erase(0, trimmed.find_first_not_of('\t'));
	trimmed.erase(trimmed.find_last_not_of('\t') + 1);
	trimmed.erase(0, trimmed.find_first_not_of(' '));
	trimmed.erase(trimmed.find_last_not_of(' ') + 1);
	trimmed.erase(trimmed.find_last_not_of(':') + 1);

	return trimmed;
}

enum requestCMD	UserRequest::isCMD(const std::string& str) {

	std::map<e_cmd, std::string>::iterator it = _enumToString.begin();

	for (; it != _enumToString.end(); it++) {
		if (it->second == str) {
			return it->first;
		}
	}
	return NONE;
}

std::string	UserRequest::handleCtrl_D(const std::string& str) {

	std::string result = str;

	result.erase(std::remove(result.begin(), result.end(), '\x04'), result.end());

	std::string::size_type pos = 0;
	while ((pos = result.find("^D", pos)) != std::string::npos) {
		result.erase(pos, 2);
	}
	return result;
}

/* DEBUG */
void	UserRequest::printInHEX(char *buff, int len) {

	// std::cout << "[";
	for (int i = 0; i < len; i++) {
		//std::cout << std::hex << (int)buff[i] << " ";
		std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)buff[i];
		if (i < len - 1) {
			std::cout << " ";
		}
	}
	// std::cout << "]" << std::endl;
	std::cout << std::endl;
}

void	UserRequest::printCommands() {

	std::cout << YELLOW << "Printing commands.." << RESET << std::endl;

	for (MapIterator it = _commandsFromClient.begin(); it != _commandsFromClient.end(); it++) {
		std::cout << "\tkey: [" << MAGENTA << it->first << RESET << "] value: [" << CYAN << it->second << RESET << "]" << std::endl;
	}
	// std::cout << RESET << std::endl;
}
/* ****** */
