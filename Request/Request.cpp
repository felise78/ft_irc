#include "Request.hpp"

Request::Request();
{
    std::cout << "Request constructor called" << std::endl;
}

Request::~Request()
{
    std::cout << "Request destructor called" << std::endl;
}

std::string const&	Request::getMessage() const
{
    std::map<std::string, std::string>::iterator it = _input_map.find("message");
    if (it != _input_map.end())
        return (it->second);
    return (NULL);
}
		
std::string const&	Request::getCommand() const
{
    std::map<std::string, std::string>::iterator it = _input_map.find("command");
    if (it != _input_map.end())
        return (it->second);
    return (NULL);
}

std::string const&	Request::getPrefix() const
{
    std::map<std::string, std::string>::iterator it = _input_map.find("prefix");
    if (it != _input_map.end())
        return (it->second);
    return (NULL);
}

std::string const&  Request::getParams() const
{
    std::map<std::string, std::string>::iterator it = _input_map.find("parameter");
    if (it != _input_map.end())
        return (it->second);
    return (NULL);
}

void  Request::set_to_map()
{
    if (!_input_buffer)
        return ; // Throw exception?
    for (int i = 0; _input_buffer[i] != 0; i++)
    {
        if (_input_buffer[0] == ':')
            
    }
}