#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include "Server.hpp"

#include <arpa/inet.h> // inet_ntoa
#include <fcntl.h> // fcntl
#include <map> // std::map
#include <list> // std::list
#include <ctime> // std::time_t
#include <netinet/in.h>

/*
** This struct is to store the client's data
** We might make is a separate class if needed..
*/
typedef struct s_client {
	
	// int				serverFd; // not necessary if we have only 1 server !
	int				port;
	std::string		hostName;
	std::string		nickName;
	std::string		userName;
	std::string		realName;
	std::string		clientMessageBuffer;
	std::string		responseBuffer; // not sure if this is needed

} t_client;

class ServerManager {

	private:
		Server						_server;

		fd_set						_recv_fd_pool; // To store the socket FDs of the clients
		fd_set						_send_fd_pool; // To store the socket FDs of the clients
		int							_max_fd; // To store the max socket FD

		void						_fcntl();
		void						_addToSet(int fd, fd_set *set);
		void						_removeFromSet(int fd, fd_set *set);
		void						_closeConnection(int fd);

		void						_accept();
		void						_handle(int fd);
		void						_respond(int fd);

	public:

		ServerManager();
		~ServerManager();

		// `clientMap` key is the client's socket FD and the value is the buffer struct (t_buffer)
		std::map<int, t_client>		clientsMap;

		// Main logic to run the servers, receive, handle and respond to the requests
		std::string					timeStamp();
		void						checkErrorAndExit(int returnValue, const std::string& msg);
		void						run();

		void						addClient(int clientFd, struct sockaddr_in &address);
		void						log(int clientFd);

};

#endif
