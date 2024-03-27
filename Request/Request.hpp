#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <map>
#include <iostream>
#include <sstream>
#include <vector>
#include <cctype>
#include "../Commands/CommandHandler.hpp"
#include "../User/User.hpp"

class ServerManager;

class Request
{
	private:
		ServerManager&							_server;							
		std::string                         	_input_buffer;
		std::map<std::string, std::string>		_input_map;
		bool									_request_valid;
		//UTILS
		void	check_command_valid(std::string& command);
		void	remove_empty_elements();

	
	public:
		Request(ServerManager& server, std::string buffer);
		~Request();
		//SETTERS
		void	parse_args();
		void	parse_params();
		void 	set_to_map(std::vector<std::string>& split_buffer);
		//GETTERS
		std::string const&								getCommand() const;
		std::map<std::string, std::string>&				getRequestMap();
		bool											getRequestValid() const;
};

#endif
