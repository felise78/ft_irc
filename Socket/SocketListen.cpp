#include "SocketListen.hpp"

SocketListen::SocketListen() :
	_socket_fd(-1) {

	std::cout << MAGENTA << "\t[ SocketListen ] constructor called." << RESET << std::endl;
}

SocketListen::~SocketListen() {

	std::cout << RED << "\t[~] SocketListen destructor called." << RESET << std::endl;

	// This might leak when server is interrupted with ctrl+c
	close(_socket_fd);
}

int		SocketListen::getSocketFd() const {

	return (_socket_fd);
}

void	SocketListen::initSocket(int domain, int service, int protocol, int port, u_long interface) {

	// Defining address structure
	_address.sin_family = domain; // AF_INET;
	_address.sin_port = htons(port); // htons(PORT);
	_address.sin_addr.s_addr = htonl(interface); // INADDR_ANY;

	_socket_fd = socket(domain, service, protocol); // AF_INET = IPv4, SOCK_STREAM = TCP, 0 = IP

	if (_socket_fd < 0) {
		perror("In SocketListen::initSocket()");
		exit(EXIT_FAILURE);
	}

	std::cout << GREEN << "\t[+] Socket created successfully. _socket_fd = " << this->_socket_fd << RESET << std::endl; 
}

void	SocketListen::connectToNetwork() {

	int	bindStatus = -1;
	int opt = 1;

	// `SO_REUSEADDR` will allow to use socket right after the program exits without waiting for the operating system to clean up the socket
	if (setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
		std::cerr << RED << "\t[-] Error setting socket options.. setsockopt() failed." << RESET << std::endl;
		exit(EXIT_FAILURE);
	}

	bindStatus = bind(_socket_fd, (struct sockaddr *)&_address, sizeof(_address));

	if (bindStatus < 0) {
		perror("In SocketListen::connectToNetwork()");
		exit(EXIT_FAILURE);
	}
}

void	SocketListen::startListenToNetwork(int backlog) {

	int	listenReturn = listen(_socket_fd, backlog);

	if (listenReturn < 0) {
		perror("In SocketListen::startListenToNetwork()");
		exit(EXIT_FAILURE);
	}
}
