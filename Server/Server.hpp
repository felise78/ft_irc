#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <cstdio> // perror()
#include <cstdlib> // exit()
#include <unistd.h> // close()
#include <sys/socket.h> // socket(), bind(), listen(), accept()
#include <netinet/in.h> // sockaddr_in, INADDR_ANY

// SOME COLORS FOR MAKING THE LIFE BRIGTHER !!!
#define RED		"\033[1;31m"
#define GREEN	"\033[1;32m"
#define YELLOW	"\033[1;33m"
#define BLUE	"\033[1;34m"
#define MAGENTA	"\033[1;35m"
#define CYAN	"\033[1;36m"
#define RESET	"\033[0m"

#define PROTOCOL	0
#define BACKLOG		100 // The maximum length of the queue of pending connections

/* THE FOLLOWING DATA SHALL BE PARSED FROM MAIN argv ..*/
#define PORT 		55555
#define SERVER_NAME	"localhost"
#define SERVER_PASS "password"


class Server {

	private:

		struct sockaddr_in	_address;
		int					_socket_fd;
		int					_port;
		std::string			_serverName;
		std::string			_serverPassword;

	public:
		Server();
		~Server();

		// SET
		void				setPort(const int & port);
		void				setServerName(const std::string & serverName);
		void				setServerPassword(const std::string & serverPassword);

		// GET
		int					getServerFd() const;
		int					getPort() const;
		std::string const &	getServerName() const;
		std::string const&	getServerPassword() const;

		// SOCKET HANDLING
		void				initSocket(); // socket():
		void				connectToNetwork(); // bind():
		void				startListenToNetwork(); // listen():

		bool				passwordVerified(std::string const & pass) const;

		/* DEBUG */
		void				printServerData() const;
		/* ***** */

};

#endif
