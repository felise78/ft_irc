#include "UserRequestParsing.hpp"

UserRequestParsing::UserRequestParsing(ServerManager& srv, User & user) 
	: _server(srv), _user(user) {

	/* DEBUG 
	std::cout << CYAN << "\tuserMessageBuffer: " << RESET << std::endl;
	std::cout << MAGENTA << _user.userMessageBuffer << RESET;
	std::cout << CYAN << "\tuserMessageBuffer in HEX: ('\\r' character in hex is [0d] and '\\n' is [0a])" << RESET << std::endl;
	std::cout << MAGENTA;
	printInHEX((char *)_user.userMessageBuffer.c_str(), _user.userMessageBuffer.length());
	std::cout << RESET;
	*/
	/* ***** */

	basicParsing(_user.userMessageBuffer); // filling in the `_commandsFromClient` map here

	printCommands(); // DEBUG

	// instantiating a CommandHandler object to handle the commands from the client
	CommandHandler		commandHandler(_server, _user, _commandsFromClient);
}

UserRequestParsing::~UserRequestParsing() {
}

void	UserRequestParsing::basicParsing(const std::string& buffer) {

	std::istringstream	ss(buffer);
	std::string			line;

	while (std::getline(ss, line, '\n')) {

		line = trim(line);

		lineParsing(line);
	}

	// This will remove the `\r\n` from the end of the buffer..
	_user.userMessageBuffer = trim(_user.userMessageBuffer);
}

/*
** Here we save each line of the request in the _commandsFromClient map as a key-value pair
** The `key` is the first word of the line and the `value` is the rest of the line
** The Command/key is converted to upper case and `/` is removed if it exists
*/
void	UserRequestParsing::lineParsing(const std::string& line) {

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
** `trim()` removes unnecessary spaces and new line characters etc.
*/
std::string	UserRequestParsing::trim(const std::string& str) {

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

std::string	UserRequestParsing::handleCtrl_D(const std::string& str) {

	std::string result = str;

	result.erase(std::remove(result.begin(), result.end(), '\x04'), result.end());

	std::string::size_type pos = 0;
	while ((pos = result.find("^D", pos)) != std::string::npos) {
		result.erase(pos, 2);
	}
	return result;
}


/* 
** DEBUG 
*/
void	UserRequestParsing::printInHEX(char *buff, int len) {

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

void	UserRequestParsing::printCommands() {

	// std::cout << YELLOW << "Printing commands.." << RESET << std::endl;

	std::map<std::string, std::string>::iterator it = _commandsFromClient.begin();

	for (; it != _commandsFromClient.end(); it++) {
		std::cout << "\tkey: [" << MAGENTA << it->first << RESET << "] value: [" << CYAN << it->second << RESET << "]" << std::endl;
	}
	// std::cout << RESET << std::endl;
}
/* ****** */
