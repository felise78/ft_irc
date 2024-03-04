#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include "Server.hpp"
#include "../Request/UserRequestParsing.hpp"
#include "UserResponse.hpp"
#include "../User/User.hpp"
#include <sys/types.h>  // for u_long
#include <sys/select.h> // for fd_set
#include <arpa/inet.h> // inet_ntoa
#include <fcntl.h> // fcntl
#include <map> // std::map
#include <list> // std::list
#include <ctime> // std::time_t
#include <netinet/in.h>

#define BUF_SIZE	10240
#define MSG_SIZE	512 // 512 bytes is the maximum length of a message in the IRC protocol

/*
** This class here is for testing and easier further integration ..
class User {
	
	private:
		int				port;
		int				socket;
		std::string		hostName; // ..parsed in `UserRequestParsing` class..
		std::string		nickName; // ..parsed in `UserRequestParsing` class..
		std::string		userName; // ..parsed in `UserRequestParsing` class..
		std::string		password; // ..parsed in `UserRequestParsing` class..
		bool			_authenticated; // ..to use for new User verification (NICK, USER, PASS)
		bool			_handshaked; // ..to use for composing the first response message to the client (RPL_WELCOME, RPL_YOURHOST, RPL_CREATED, RPL_MYINFO..) 
		
	public:
		std::string		userMessageBuffer;
		std::string		responseBuffer;

		User() : port(0), socket(0), hostName(""), nickName(""), userName(""), password(""), _authenticated(false), _handshaked(false) {};
		~User() {};

		// Setters
		void			setPort(int & port) { this->port = port; }
		void			setSocket(int & socket) { this->socket = socket; }
		void			setHostName(std::string const & hostName) { this->hostName = hostName; }
		void			setNickName(std::string const & nickName) { this->nickName = nickName; }
		void			setUserName(std::string const & userName) { this->userName = userName; }
		void			setPassword(std::string const & password) { this->password = password; }
		void			setAuthenticated(bool authenticated) { this->_authenticated = authenticated; }
		void			setHandshaked(bool handshaked) { this->_handshaked = handshaked; }

		// Getters
		const int &		getPort() const { return this->port; }
		const int &		getSocket() const { return this->socket; }
		const std::string &	getHostName() const { return this->hostName; }
		const std::string &	getNickName() const { return this->nickName; }
		const std::string &	getUserName() const { return this->userName; }
		const std::string &	getPassword() const { return this->password; }
		bool			authenticated() { return this->_authenticated; }
		bool			handshaked() { return this->_handshaked; }

};
*/
/* ***** */

class ServerManager {

	private:
		Server						_server;

		fd_set						_recv_fd_pool; // To store the socket FDs of the Users
		fd_set						_send_fd_pool; // To store the socket FDs of the Users
		int							_serverFd; // The server's socket FD
		int							_max_fd; // To track the max value of the socket FD, needed for `select()` function and for loop in `run()`

		// Main logic to run the servers, receive, handle and respond to the requests
		void						_run();
		void						_accept(int UserFd);
		void						_handle(int fd);
		void						_respond(int fd);

		// Helper functions
		void						_fcntl();
		void						_addToSet(int fd, fd_set *set);
		void						_removeFromSet(int fd, fd_set *set);
		void						_closeConnection(int fd);

	public:
		// `UserMap` key is the User's socket FD and the value is the User object
		std::map<int, User>			usersMap; // int is fd 
		// Add Channel map here of all created channels
		std::map<std::string, Channel *>	channelMap;

		ServerManager();
		~ServerManager();

		// Initializing User's data. Command Passing.
		// Maybe we can moove this logic to the User class.. ?!
		void						initUser(int UserFd, struct sockaddr_in &address);

		// General Helpers
		std::string					timeStamp();
		void						checkErrorAndExit(int returnValue, const std::string& msg);
		void						log(int UserFd);
		bool						isClient(int fd);
		void						setChannel(Channel& channel);
		Channel& 					getChannel( const std::string& name ) const;
		int 					getFdbyNickName( const std::string& nickname ) const;

};

#endif
