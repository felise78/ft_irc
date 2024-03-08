#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include "Server.hpp"
#include "../Commands/CommandHandler.hpp"
#include "../Request/UserRequestParsing.hpp"
#include "../Request/Request.hpp"
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
#include <signal.h> // For signal handling

#define BUF_SIZE	10240
#define MSG_SIZE	512 // 512 bytes is the maximum length of a message in the IRC protocol

class ServerManager {

	private:
		Server						_server;

		fd_set						_recv_fd_pool; // To store the socket FDs of the Users
		fd_set						_send_fd_pool; // To store the socket FDs of the Users
		int							_serverFd; // The server's socket FD
		int							_max_fd; // To track the max value of the socket FD, needed for `select()` function and for loop in `run()`
		bool						_sigInt; // To control the main loop of the server

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
		std::map<int, User>				usersMap; // int is fd 
		// Add Channel map here of all created channels
		std::map<std::string, Channel>	channelMap;
		int								error;

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
		void						setChannel(const Channel& channel);
		Channel& 					getChannel( const std::string& name );
		int 						getFdbyNickName( const std::string& nickname ) const;

		// All this is necessary for the signal handling (to be able to close the socket and exit properly)
		// `signaHandler()` must be static, as well as anything it operates on.
		// `SM_instance` is a pointer to the ServerManager instance, so that the `signalHandler` can call `handleSignal()`.
		static 	ServerManager*	SM_instance;
		static void	signalhandler(int signal);
		void		handleSignal();

};

std::vector<std::string> split(const std::string& input, const std::string& delimiter);
void	trim(std::string &str, std::string delimiter);
int		noCRLFinBuffer(std::string const& buffer);

#endif
