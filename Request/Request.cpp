#include "Request.hpp"

/*CONSTRUCTORS/DESTRUCTORS*/
Request::Request(std::string buffer, User& user) : _input_buffer(buffer), _user(user), _request_valid(false)
{
	if (DEBUG)
		std::cout << "Request constructor called" << std::endl;
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
		throw std::runtime_error("message buffer empty");
	// trim buffer of slash and carriage return 
	std::stringstream	ss(_input_buffer);
	std::string			newString;
	std::vector<std::string>	split_buffer; //splitting all args by space
	while (std::getline(ss, newString, ' '))
	{
		split_buffer.push_back(newString);
	}
	set_to_map(split_buffer);
}

void	split_commas(std::multimap<std::string, std::string>& _input_map);
void  Request::set_to_map(std::vector<std::string>& split_buffer)
{
	typedef std::vector<std::string>::iterator vector_it;
	typedef std::multimap<std::string, std::string>::iterator map_it;
	vector_it it = split_buffer.begin();
	_input_map.insert(std::make_pair("command", *it)); //first arg is always command, ignore prefix case?
	it++;
	while (it != split_buffer.end())
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
			if (it2 == _input_map.end())
				throw std::runtime_error("no command found");
			else if (it2->second == "NICK" || it2->second == "PASS"
				|| it2->second == "USER")
				_input_map.insert(std::make_pair("arg", *it)); // elements of map with "arg" as key are args for PASS, NICK and USER commands
			else
				_input_map.insert(std::make_pair("user", *it)); // user to whom command is destined
			it++;
		}
	}
	split_commas(_input_map);
	_request_valid = true; //while unused, needed for compilation
	(void) _user; //while unused, needed for compilation
	if (DEBUG)
	{
		std::cout << "Printing map" << std::endl;
		print_map();
		std::cout << std::endl;
	}
}

void	split_commas(std::multimap<std::string, std::string>& _input_map)
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

/*GETTERS*/
std::string const&	Request::getCommand() const
{
    std::map<std::string, std::string>::const_iterator it = _input_map.find("command");
    if (it != _input_map.end())
        return (it->second);
	else
		throw std::runtime_error("no command found");
}

// std::string const&	Request::getPrefix() const
// {
//     std::map<std::string, std::string>::const_iterator it = _input_map.find("prefix");
//     if (it != _input_map.end())
//         return (it->second);
// 	else
// 		throw std::runtime_error("no prefix found");
// }

/*DEBUG*/

void	Request::print_map() const
{
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
// 	User	user;
// 	Request test("KICK #general,#channel1 johndoe,johndoe1,johndoe2 :johndoes are idiots", user);
// 	Request test2("JOIN #t1,#t2", user);
// 	Request test3("NICK nickname", user);

// }