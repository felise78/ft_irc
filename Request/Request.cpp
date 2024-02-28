#include "Request.hpp"

/*CONSTRUCTORS/DESTRUCTORS*/
Request::Request(std::string buffer, User& user) : _input_buffer(buffer), _user(user), _request_valid(false)
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
	std::stringstream	ss(_input_buffer);
	std::string			newString;
	std::vector<std::string>	split_buffer; //splitting all args by space
	while (std::getline(ss, newString, ' '))
	{
		split_buffer.push_back(newString);
	}
	set_to_map(split_buffer);
}

void  Request::set_to_map(std::vector<std::string>& split_buffer)
{
	typedef std::vector<std::string>::iterator vector_it;
	typedef std::multimap<std::string, std::string>::iterator map_it;
	vector_it it = split_buffer.begin();
	check_command_valid(*it); //all commands capitalised here
	_input_map.insert(std::make_pair("command", *it)); //first arg is always command, ignore prefix case?
	it++;
	while (_request_valid && it != split_buffer.end())
	{
		if (((*it)[0] == '#') || ((*it)[0] == '&'))
		{
			_input_map.insert(std::make_pair("channel", *it)); //can be multiple channels
			it++;
		}
		else if ((*it)[0] == ':')
		{
			std::string param; 
			while (it != split_buffer.end())
			{
				param += *it;
				param += " ";
				it++;
			}
			param.erase(0,1); //removes the : at the beginning
			_input_map.insert(std::make_pair("message", param)); //if an arg starts with :, everything that follows is message
		}
		else if ((*it)[0] == '+' || (*it)[0] == '-')
		{
			_input_map.insert(std::make_pair("flag", *it)); //flags for MODES command
			it++;
		}
		else
		{
			map_it it2 = _input_map.find("command");
			map_it flag_it = _input_map.find("flag");
			if (it2 == _input_map.end())
				throw std::runtime_error("no command found");
			else if (it2->second == "NICK" || it2->second == "PASS"
				|| it2->second == "USER" 
				|| (it2->second == "MODE" && flag_it != _input_map.end() && flag_it->second == "+k"))
				_input_map.insert(std::make_pair("arg", *it)); // elements of map with "arg" as key are args for PASS, NICK and USER commands, or password for mode +k parameter
			else
				_input_map.insert(std::make_pair("user", *it)); // user to whom command is destined
			it++;
		}
	}
	split_commas(_input_map); // if multiple channels or users in command, they are separated by ,
	remove_empty_elements(); //if multiple spaces between args
	(void) _user; //while unused, needed for compilation
	if (DEBUG)
	{
		print_map();
		std::cout << std::endl;
	}
}

/* UTILS */
void	Request::split_commas(std::multimap<std::string, std::string>& _input_map)
{
	typedef std::multimap<std::string, std::string>::iterator map_it;

	for (map_it it2 = _input_map.begin(); it2 != _input_map.end();)
	{
		if (it2->second.find(',') != std::string::npos)
		{
			std::stringstream	ss(it2->second);
			std::string			newString;
			while (std::getline(ss, newString, ','))
			{
				_input_map.insert(std::make_pair(it2->first, newString));
			}
			it2 = _input_map.erase(it2);
		}
		else
			it2++;
	}
}

void	Request::check_command_valid(std::string& command)
{
	for (size_t i = 0; i < command.length(); i++)
		command[i] = toupper(command[i]);
	if (DEBUG)
		std::cout << command << std::endl;
	for (t_commands i = KICK; i <= PART;)
	{
		if (command == _commands_map[i])
		{
			_request_valid = true;
			if (DEBUG)
				std::cout << "_request_valid is set as " << std::boolalpha << _request_valid << std::endl;
			return ;
		}
		i = static_cast<t_commands>(static_cast<int>(i) + 1);
	}
	if (DEBUG)
		std::cout << "_request_valid is set as " << std::boolalpha << _request_valid << std::endl;
}

void	Request::remove_empty_elements()
{
	typedef std::multimap<std::string, std::string>::iterator map_it;
	for (map_it it = _input_map.begin(); it != _input_map.end();)
	{
		if ((it->second).empty())
			it = _input_map.erase(it);
		else
			it++;
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

std::multimap<std::string, std::string>	const& Request::getRequestMap() const
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
// 		User	user;
// 		Request test("KICK #general,#channel1 johndoe,johndoe1,johndoe2 :johndoes are idiots", user);
// 		Request test2("JOIN #t1,#t2\r\n", user);
// 		Request test3("NICK       nickname", user);
// 		Request test4("mode #general +t", user);
// 		Request rubbish("Not a command", user);
// 		Request nospacing("MODE   +k      something", user);
// 		Request empty("", user);
// 	}
// 	catch (std::exception const& e)
// 	{
// 		std::cerr << e.what() << std::endl;
// 		return (1);
// 	}
// }
