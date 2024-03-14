#ifndef SERVER_UTILS_HPP
#define SERVER_UTILS_HPP

#include "Server.hpp"
#include "ServerManager.hpp"

std::vector<std::string> split(const std::string& input, const std::string& delimiter);
void trim(std::string &str, std::string delimiter);
int	noCRLFinBuffer(std::string const& buffer);
int	noPassInCmd(std::string const& messageBuffer);

#endif