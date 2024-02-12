#ifndef SERVER_HPP
#define SERVER_HPP

#include "../Socket/SocketListen.hpp"

/* THE FOLLOWING DEFINITIONS SHALL BE CHANGED ..*/
#define PROTOCOL	0
#define PORT 		55555
#define BACKLOG		100 // The maximum length of the queue of pending connections
#define SERVER_NAME	"localhost"
#define SERVER_PASSWORD "password"

#define BUF_SIZE	10240


class Server {

	private:
		//int										_serverFd;
		int										_port;
		std::string								_serverName;
		std::string								_serverPassword;

		SocketListen							_serverSocket;

	public:
		Server();
		~Server();

		int					getServerFd() const;
		std::string			getServerName() const;

		void				initServerSocket();

		/* DEBUG */
		void				printServerData() const;
		/* ***** */

};

#endif
