#include "Request.hpp"

/*CONSTRUCTORS/DESTRUCTORS*/
Request::Request(std::string buffer) : _input_buffer(buffer)
{
	if (DEBUG)
		std::cout << "Request constructor called" << std::endl;
}

Request::~Request()
{
	if (DEBUG)
		std::cout << "Request destructor called" << std::endl;
}

/*SETTERS*/
int		Request::set_prefix(std::string const& param)
{
	if (param.empty())
		throw std::runtime_error("command not found");
	if (param[0] == ':')
	{
		_input_map.insert(std::make_pair("prefix", param));
		return (0);
	}
	return (1);
}

void	Request::set_command(std::string const& param)
{
	if (param.empty())
		return ;
	_input_map.insert(std::make_pair("command", param));
}

#include <stdio.h>
void	Request::set_params(std::vector<std::string>& split_buffer)
{
	if (split_buffer.empty())
		return ;
	typedef std::vector<std::string>::const_iterator it;
	for (it i = split_buffer.begin(); i != split_buffer.end();)
	{
		_input_map.insert(std::make_pair("param", *i));
		printf("%s\n", i->c_str());
		i = split_buffer.erase(i);
		printf("%s\n", i->c_str());
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

std::string const&	Request::getPrefix() const
{
    std::map<std::string, std::string>::const_iterator it = _input_map.find("prefix");
    if (it != _input_map.end())
        return (it->second);
	else
		throw std::runtime_error("no prefix found");
}

/*OTHER*/

void  Request::set_to_map()
{
    if (_input_buffer.empty())
		return ; // Throw exception?
	std::stringstream	ss(_input_buffer);
	std::string			newString;
	std::vector<std::string>	split_buffer;
	while (std::getline(ss, newString, ' '))
	{
		if (set_prefix(newString))
		{
			set_command(newString);
			continue;
		}
		
	}
	// set_prefix(split_buffer);
	// set_command(split_buffer);
	set_params(split_buffer);
	if (DEBUG)
		print_map();

}

void	Request::print_map() const
{
	std::map<std::string, std::string>::const_iterator it;
	for (it = _input_map.begin(); it != _input_map.end(); ++it)
	{
		std::cout << "Key: " << it->first << "\nValue: " << it->second << std::endl;
	}
}

// int	main(void)
// {
// 	// Request test("KICK #general johndoe :shithead");

// 	// test.set_to_map();
// 	std::string test = "Here is a sentence with all these words";
// 	std::string newString;
// 	std::stringstream	stream(test);
// 	while (std::getline(stream, newString, 'r'))
// 	{
// 		std::cout << newString << std::endl;
// 	}
// }