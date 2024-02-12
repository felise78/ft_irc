#include "Server.hpp"

Server::Server() : 
	_port(PORT),
	_serverName(SERVER_NAME),
	_serverPassword(SERVER_PASSWORD) {

	std::cout << MAGENTA << "\tServer constructor called" << RESET << std::endl;
}

Server::~Server() {

	std::cout << RED << "\tServer destructor called" << RESET << std::endl;
}

/*
** This GETTER returns the `_socket_fd` value from `SocketListen` class.
*/
int		Server::getServerFd() const {

	return _serverSocket.getSocketFd();
}

std::string	Server::getServerName() const {

	return _serverName;
}

void	Server::initServerSocket() {

	_serverSocket.initSocket(AF_INET, SOCK_STREAM, PROTOCOL, _port, INADDR_ANY);
	_serverSocket.connectToNetwork();
	_serverSocket.startListenToNetwork(BACKLOG);
}

/*
** This function is for testing purposes only !!!
** PRINTS THE SERVER DATA
*/
void	Server::printServerData() const {

	std::cout << YELLOW << "[!] Server DATA:" << RESET << std::endl;
	std::cout << YELLOW << "\tServer fd: " << _serverSocket.getSocketFd() << RESET << std::endl;
	std::cout << YELLOW << "\tServer port: " << _port << RESET << std::endl;
	std::cout << YELLOW << "\tServer name: " << _serverName << RESET << std::endl;
	std::cout << YELLOW << "\tServer password: " << _serverPassword << RESET << std::endl;

}
