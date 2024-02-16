#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include "Server.hpp"

#include <sys/types.h>  // for u_long
#include <sys/select.h> // for fd_set
#include <arpa/inet.h> // inet_ntoa
#include <fcntl.h> // fcntl
#include <map> // std::map
#include <list> // std::list
#include <ctime> // std::time_t
#include <netinet/in.h>

#define BUF_SIZE	10240

/*
** This struct is to store the client's data
** This class here is for testing and easier further integration ..
*/
class Client {
	
	private:
		int				port;
		int				socket;
		std::string		hostName;
		std::string		nickName;
		std::string		userName;
		std::string		password;
		
	public:
		std::string		clientMessageBuffer;
		std::string		responseBuffer;

		Client() : port(0), socket(0), hostName(""), nickName(""), userName(""), password("") {};
		~Client() {};

		// Setters
		void			setPort(int & port) { this->port = port; }
		void			setSocket(int & socket) { this->socket = socket; }
		void			setHostName(std::string const & hostName) { this->hostName = hostName; }
		void			setNickName(std::string const & nickName) { this->nickName = nickName; }
		void			setUserName(std::string const & userName) { this->userName = userName; }
		void			setPassword(std::string const & password) { this->password = password; }

		// Getters
		const int &		getPort() const { return this->port; }
		const int &		getSocket() const { return this->socket; }
		const std::string &	getHostName() const { return this->hostName; }
		const std::string &	getNickName() const { return this->nickName; }
		const std::string &	getUserName() const { return this->userName; }
		const std::string &	getPassword() const { return this->password; }

};
/* ***** */

class ServerManager {

	private:
		Server						_server;

		fd_set						_recv_fd_pool; // To store the socket FDs of the clients
		fd_set						_send_fd_pool; // To store the socket FDs of the clients
		int							_serverFd; // The server's socket FD
		int							_max_fd; // To track the max value of the socket FD, needed for `select()` function and for loop in `run()`

		// Main logic to run the servers, receive, handle and respond to the requests
		void						_run();
		void						_accept(int clientFd);
		void						_handle(int fd);
		void						_respond(int fd);

		// Helper functions
		void						_fcntl();
		void						_addToSet(int fd, fd_set *set);
		void						_removeFromSet(int fd, fd_set *set);
		void						_closeConnection(int fd);

	public:
		// `clientMap` key is the client's socket FD and the value is the Client object
		std::map<int, Client>		clientsMap;

		ServerManager();
		~ServerManager();

		// Initializing User's data. Command Passing.
		// Maybe we can moove this logic to the User class..
		void						initClient(int clientFd, struct sockaddr_in &address);

		// General Helpers
		std::string					timeStamp();
		void						checkErrorAndExit(int returnValue, const std::string& msg);
		void						log(int clientFd);
		bool						isClient(int fd);

		// TESTING
		// void						composeResponse(int fd);

};

#endif
