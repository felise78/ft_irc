#ifndef IRCBOT_HPP
#define IRCBOT_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <cstring> // For memset()
#include <sys/socket.h> // For socket(), connect(), send(), and recv()
#include <netdb.h> // For gethostbyname()
#include <unistd.h> // For close()
#include <signal.h> // For signal handling
#include <stdlib.h> // For exit()
#include <fcntl.h> // For fcntl()
#include <arpa/inet.h> // For inet_addr() and inet_pton() ..converts IP addresses from text to binary form

// SOME COLORS FOR MAKING THE LIFE BRIGTHER !!!
#define RED		"\033[1;31m"
#define GREEN	"\033[1;32m"
#define YELLOW	"\033[1;33m"
#define BLUE	"\033[1;34m"
#define MAGENTA	"\033[1;35m"
#define CYAN	"\033[1;36m"
#define RESET	"\033[0m"


#define			BUFFER_SIZE 1024
extern bool		signalFlag;

class IrcBot {

private:

    int			_serverSocket;
	int			_serverPort;
	std::string	_serverName;
	std::string	_serverPass;
    std::string	_botName;
	std::string	_serverRequestBuffer; // Buffer for server requests
	std::string	_responseGPT; // Buffer for GPT response

public:

	bool		_authenticated; // ..to use for new User verification (NICK, USER, PASS)
	bool		signalErrorFlag;

    IrcBot(const std::string& serverName, int port, const std::string& pass, const std::string& botName);
    ~IrcBot();

	void		initSocket();
    void		connectToServer();

	void		sendHandshake();
    void		joinChannel(const std::string& channel);
    void		sendIrcMessage(const std::string& message);
    void		sendMessage(const std::string& channel, const std::string& message);

    void		handleServerRequest();	
	void		handleResponse();
	void		handleGPT();

	// The following declarations are needed for the signal handling (to be able to close the socket and exit properly)
	// ..the signalHandler() must be static, as well as anything it operates on.
	// ..the ircBotInstance is a pointer to `IrcBot` instance, this way it can be used in the `signalHandler()` to call `handleSignal()`.
	static 	IrcBot*	ircBotInstance;
	static void	signalHandler(int signal);
	void		handleSignal();

	void		checkErrorAndExit(int returnValue, const std::string& message);

};

#endif
