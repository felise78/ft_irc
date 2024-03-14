#include "Server.hpp"
#include "ServerManager.hpp"

// split splits string input at every occurence of string delimiter, and returns a vector
std::vector<std::string> split(const std::string& input, const std::string& delimiter) 
{
    std::vector<std::string> tokens;
    size_t lastPos = 0;
    size_t pos = input.find(delimiter, lastPos);
    while (pos != std::string::npos)
	{
		string str = input.substr(lastPos, pos - lastPos);
		if (!str.empty()) {
			tokens.push_back(str);}
		lastPos = pos + delimiter.length();
		pos = input.find(delimiter, lastPos);
    }
	string str = input.substr(lastPos);
	if (!str.empty()) {
    	tokens.push_back(str);}
    return tokens;
}

void trim(std::string &str, std::string delimiter)
{
	size_t start = str.find_first_not_of(delimiter);
    if (start != std::string::npos) {
		str = str.substr(start);
    } 
	else {
		str.clear(); // Entire string is whitespace
        return;
    }
    size_t end = str.find_last_not_of(delimiter);
    if (end != std::string::npos) {
        str = str.substr(0, end + 1);
    } else {
        str.clear(); // Entire string is whitespace
    }
}

int	noCRLFinBuffer(std::string const& buffer)
{
	size_t crlf = buffer.find("\n");
	if (crlf == std::string::npos)
		return 1;
	return 0;
}

int	noPassInCmd(std::string const& messageBuffer)
{
	if (messageBuffer.find("\npass ") != std::string::npos)
		return 0;
	if (messageBuffer.find("\nPASS ") != std::string::npos)
		return 0;
	return 1;
}