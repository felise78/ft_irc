#include "ClientRequest.hpp"

/*
*/
ClientRequest::ClientRequest(const std::string& buffer) {

	std::istringstream	ss(buffer);
	std::string			line;

	// Parsing each line into map by the first space
	while (std::getline(ss, line) && !line.empty() && line != "\r") {

		line = trim(line);

		_basicParsing(line);
	}

	// Identifying the first FIRST WORD of the each line in the request
	// _extractCMD();

}

ClientRequest::~ClientRequest() {
}

/*
** Here we save each line of the request in the _commands map as a key-value pair
** The `key` is the first word of the line and the `value` is the rest of the line
*/
void	ClientRequest::_basicParsing(const std::string& line) {
	//std::cout << CYAN << "in parseLine().." << RESET << std::endl;

	std::istringstream	ss(line);
	std::string			firstWord;
	std::string			theRest;

	std::getline(ss, firstWord, ' ');
	std::getline(ss, theRest);
	
	firstWord = trim(firstWord);
	theRest = trim(theRest);

	if (firstWord.length() != 0 || theRest.length() != 0) {
		// _commands.insert(std::make_pair(firstWord, theRest));
		_commands[firstWord] = theRest;
	}

}

/*
** using typedef on the map iterator to make it easier to read
**
** Here we iterate through the _commands map and extract/identify the first word of the line as a COMMAND
*/
typedef std::map<std::string, std::string>::iterator MapIterator;
void	ClientRequest::_extractCMD() {

	for (MapIterator it = _commands.begin(); it != _commands.end(); it++) {

		// Checking if the first word of the line is a CMD (enum requestCMD), NONE = 0
		if (isCMD(it->first)) {
			_CMD = isCMD(it->first);
			
			/* DEBUG */
			// std::cout << YELLOW << "\tCMD: " << getCMD() << RESET << std::endl;
			/* ***** */
		}
	}
}

// Getters
std::string	ClientRequest::getCMD() {
	
	if (_CMD == CAP) {
		return "CAP";
	}
	if (_CMD == NICK) {
		return "NICK";
	}
	else if (_CMD == USER) {
		return "USER";
	}
	else {
		return "NONE";
	}
}

/*
** Clean parsing helpers.
**
** `trim()` removes unnecessary spaces and new line characters etc.
*/
std::string	ClientRequest::trim(const std::string& str) {

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

enum requestCMD	ClientRequest::isCMD(const std::string& str) {

	if (str == "CAP") {
		return CAP;
	}
	else if (str == "NICK") {
		return NICK;
	}
	else if (str == "USER") {
		return USER;
	}
	else {
		return NONE;
	}
}

void	ClientRequest::printInHEX(char *buff, int len) {

	std::cout << "[";
	for (int i = 0; i < len; i++) {
		//std::cout << std::hex << (int)buff[i] << " ";
		std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)buff[i] << " ";
	}
	std::cout << "]" << std::endl;
}

/* DEBUG */
void	ClientRequest::printCommands() {

	std::cout << YELLOW << "Printing commands.." << RESET << std::endl;

	for (MapIterator it = _commands.begin(); it != _commands.end(); it++) {
		std::cout << "\tkey: [" << MAGENTA << it->first << RESET << "] value: [" << CYAN << it->second << RESET << "]" << std::endl;
	}
	// std::cout << RESET << std::endl;
}
