#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include "Server.hpp"

#include <arpa/inet.h> // inet_ntoa
#include <fcntl.h> // fcntl
#include <map> // std::map
#include <list> // std::list
#include <ctime> // std::time_t

typedef struct s_buffer {
	
	int				serverFd;
	std::string		requestBuffer;
	std::string		responseBuffer;

} t_buffer;

class ServerManager {
	private:

		Server						_server;

		fd_set						_recv_fd_pool; // To store the socket FDs of the clients
		fd_set						_send_fd_pool; // To store the socket FDs of the clients
		int							_max_fd; // To store the max socket FD

		void						_initFdSets(); // ftcnl()
		void						_addToSet(int fd, fd_set *set);
		void						_removeFromSet(int fd, fd_set *set);
		void						_closeConnection(int fd);

		void						_accept(int fd);
		void						_handle(int fd);
		void						_respond(int fd);

	public:

		ServerManager();
		~ServerManager();

		std::map<int, t_buffer>		clientsMap;

		// Main logic to run the servers, receive, handle and respond to the requests
		std::string					timeStamp();
		void						checkErrorAndExit(int returnValue, const std::string& msg);
		void						run();
		bool						isClient(int fd);

};

#endif
