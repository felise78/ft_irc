#ifndef SERVER_HPP
#define SERVER_HPP

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

/* THE FOLLOWING DEFINITIONS SHALL BE CHANGED ..*/
#define PROTOCOL	0
#define PORT 		55555
#define BACKLOG		100 // The maximum length of the queue of pending connections
#define SERVER_NAME	"localhost"
#define SERVER_PASSWORD "password"

#define BUF_SIZE	10240


class Server {

	private:

		struct sockaddr_in	_address;
		int					_socket_fd; // this is the file descriptor returned by `socket()`
		int					_port;
		std::string			_serverName;
		std::string			_serverPassword;

		// SocketListen							_serverSocket;

	public:
		Server();
		~Server();

		int					getServerFd() const;
		std::string			getServerName() const;

		void				initServerSocket();

		// void	initSocket(int domain, int service, int protocol, int port, u_long interface);
		void	initSocket();
		void	connectToNetwork(); // bind():
		void	startListenToNetwork(int backlog); // listen():

		/* DEBUG */
		void				printServerData() const;
		/* ***** */

};

#endif
