#ifndef SOCKETLISTEN_HPP
#define SOCKETLISTEN_HPP

#include <iostream>
#include <string>
#include <cstdio> // perror()
#include <cstdlib> // exit()
#include <unistd.h> // close()
#include <sys/socket.h> // socket(), bind(), listen(), accept()
#include <netinet/in.h> // sockaddr_in, INADDR_ANY
#include <sys/types.h>  // for u_long
#include <sys/select.h> // for fd_set

// SOME COLORS FOR MAKING THE LIFE BRIGTHER !!!
#define RED		"\033[1;31m"
#define GREEN	"\033[1;32m"
#define YELLOW	"\033[1;33m"
#define BLUE	"\033[1;34m"
#define MAGENTA	"\033[1;35m"
#define CYAN	"\033[1;36m"
#define RESET	"\033[0m"


/*
** This class initializes the socket and listens to the network on the server side.
** The `SocketListen` class shall be the part of the `Server` class.
*/
class SocketListen {
	private:

		struct sockaddr_in	_address;
		int					_socket_fd; // this will be the file descriptor returned by `socket()`
	
	public:

		SocketListen ();
		~SocketListen ();

		int					getSocketFd() const;

		// The following functions shall be called once the instance of Server class is created and the basic data is parsed from input
		// socket():
		void	initSocket(int domain, int service, int protocol, int port, u_long interface);
		void	connectToNetwork(); // bind():
		void	startListenToNetwork(int backlog); // listen():

};

#endif