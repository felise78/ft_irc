#ifndef CLIENTREQUEST_HPP
#define CLIENTREQUEST_HPP

#include <sstream> // std::stringstream
#include <string>
#include <map>
#include <string.h>
#include <iomanip> // std::setw and std::setfill

#include "ServerManager.hpp"

#define BUF_SIZE 10240 // 100 KB to store the request from the browser

enum requestCMD {

	NONE,
	CAP,
	NICK,
	USER
};

class ClientRequest {
	private:

		enum requestCMD						_CMD;
		std::map<std::string, std::string>	_commands;

		// parsing helpers	
		void _basicParsing(const std::string& line); // To parse the headers of the HTTP request
		void _extractCMD(); // To extract the first line of the HTTP request

	public:

		ClientRequest(const std::string& requestBuffer); // Constructor reseives the request buffer from HttpServer::_handle() method
		~ClientRequest();

		// Getters
		std::string			getCMD();

		// Clean parsing helpers
		std::string			trim(const std::string& str);
		enum requestCMD		isCMD(const std::string& str);
		void				printInHEX(char *buff, int len);

		/* DEBUG */
		void				printCommands();

};

#endif
