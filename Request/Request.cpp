#include "Request.hpp"

/*CONSTRUCTORS/DESTRUCTORS*/
Request::Request(std::string buffer) : _input_buffer(buffer), _request_valid(false)
{
	if (DEBUG)
		std::cout << "Request constructor called" << std::endl;
	_commands_map[KICK] = "KICK";
	_commands_map[INVITE] = "INVITE";
	_commands_map[TOPIC] = "TOPIC";
	_commands_map[MODE] = "MODE";
	_commands_map[NICK] = "NICK";
	_commands_map[USER] = "USER";
	_commands_map[PASS] = "PASS";
	_commands_map[OPER] = "OPER";
	_commands_map[JOIN] = "JOIN";
	_commands_map[PRIVMSG] = "PRIVMSG";
	_commands_map[PART] = "PART";
	parse_args();
}

Request::~Request()
{
	if (DEBUG)
		std::cout << "Request destructor called" << std::endl;
}

/*SETTERS*/

void	Request::parse_args()
{
    if (_input_buffer.empty())
		return ;
	if (_input_buffer[0] == '/') //trimming initial slash, if it exists, and \r\n
		_input_buffer.erase(_input_buffer.begin());
	for (size_t i = _input_buffer.find_last_of('\n', '\r'); i != std::string::npos;)
	{
		_input_buffer.erase(i);
		i = _input_buffer.find_last_of('\n', '\r');
	}
	size_t i = _input_buffer.find_first_of(' ');
	if (i != std::string::npos)
	{
		_input_map.insert(std::make_pair("command", _input_buffer.substr(0, i)));
		_input_map.insert(std::make_pair("params", _input_buffer.substr(i + 1)));
		parse_params();
	}
	check_command_valid(_input_map["command"]);
	if (DEBUG)
	{
		print_map();
		std::cout << std::endl;
	}
}

/* UTILS */

void	Request::check_command_valid(std::string& command)
{
	for (size_t i = 0; i < command.length(); i++)
		command[i] = toupper(command[i]);
	if (DEBUG)
		std::cout << command << std::endl;
	std::map<e_cmd, std::string>::iterator it = _commands_map.begin();
	for (; it != _commands_map.end(); it++)
	{
		if (command == it->second)
		{
			_request_valid = true;
			if (DEBUG)
				std::cout << "_request_valid is set as " << std::boolalpha << _request_valid << std::endl;
			return ;
		}
	}
	command = "NONE";
	if (DEBUG)
		std::cout << "_request_valid is set as " << std::boolalpha << _request_valid << std::endl;
}

void	Request::parse_params()
{
	bool hash = false;

	std::string::iterator it = _input_map["params"].begin();

	for (it; it != _input_map["params"].end(); ++it)
	{
		if (*it == '#' && hash == false)
			hash = true;
		else if (*it == '#' && hash == true)
			return;
	}
}

/*GETTERS*/

bool	Request::getRequestValid() const
{
	return _request_valid;
}

std::string const&	Request::getCommand() const
{
    std::map<std::string, std::string>::const_iterator it = _input_map.find("command");
    if (it != _input_map.end())
        return (it->second);
	else
		throw std::runtime_error("no command found");
}

std::map<std::string, std::string>	const& Request::getRequestMap() const
{
	return (_input_map);
}

/*DEBUG*/
void	Request::print_map() const
{
	std::cout << "Printing input map:" << std::endl;
	std::map<std::string, std::string>::const_iterator it;
	for (it = _input_map.begin(); it != _input_map.end(); ++it)
	{
		std::cout << "Key: " << it->first << "\nValue: " << it->second << std::endl;
	}
}

void	Request::print_vector(std::vector<std::string> const& split_buffer)
{
	for (std::vector<std::string>::const_iterator it = split_buffer.begin(); it != split_buffer.end(); it++)
		std::cout << *it << std::endl;
}

// int	main(void)
// {
// 	try {
// 		Request test("KICK #general,#channel1 johndoe,johndoe1,johndoe2 :johndoes are idiots");
// 		Request test2("JOIN #t1,#t2\r\n");
// 		Request test3("NICK       nickname");
// 		Request test4("mode #general +t");
// 		Request rubbish("Not a command");
// 		Request nospacing("MODE   +k      something");
// 		Request empty("");
// 	}
// 	catch (std::exception const& e)
// 	{
// 		std::cerr << e.what() << std::endl;
// 		return (1);
// 	}
// }
