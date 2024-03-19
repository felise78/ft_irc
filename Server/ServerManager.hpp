#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include "Server.hpp"
#include "server_utils.hpp"
#include "../Commands/CommandHandler.hpp"
#include "../Request/UserRequestParsing.hpp"
#include "../Request/Request.hpp"
#include "../Request/server_replies.h"
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

	public:
		// Helper functions
		void						_fcntl();
		void						_addToSet(int fd, fd_set *set);
		void						_removeFromSet(int fd, fd_set *set);
		void						_closeConnection(int fd);

		// `UserMap` key is the User's socket FD and the value is the User object
		std::map<int, User>				usersMap; // int is fd 
		// Add Channel map here of all created channels
		std::map<std::string, Channel>	channelMap;

		ServerManager(int port, std::string const& password);
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
		std::string const&			getPassword() const;

		// These two functions are used to set the broadcast message and send it to the users
		void						setBroadcast(std::string channelName, std::string sender_nick, std::string msg); // this will be used to broadcast messages to all users in a channel
		void						setBroadcast(std::string msg, int fd); // to send message to a specific user given fd

		// All this is necessary for the signal handling (to be able to close the socket and exit properly)
		// `signaHandler()` must be static, as well as anything it operates on.
		// `SM_instance` is a pointer to the ServerManager instance, so that the `signalHandler` can call `handleSignal()`.
		static 	ServerManager*	SM_instance;
		static void	signalhandler(int signal);
		void		handleSignal();
};

#endif
