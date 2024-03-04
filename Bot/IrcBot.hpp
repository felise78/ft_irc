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

    IrcBot(const std::string& serverName, int port, const std::string& pass, const std::string& botName);
    ~IrcBot();

	void		initClientSocket();
    void		connectToServer();

	void		sendHandshake();
    void		joinChannel(const std::string& channel);
    void		sendIrcMessage(const std::string& message);
    void		sendMessage(const std::string& channel, const std::string& message);

    void		handleServerRequest();	
	void		handleResponse();
	void		handleGPT();

	static void	signalHandler(int signal);

	// Getting used to exceptions ;)
	class IrcBotException : public std::exception {
		public:
			std::string message;
			IrcBotException(const std::string& msg) : message(msg) { }
			~IrcBotException() throw() { }
			const char* what() const throw() { 
				static std::string colored_message = RED + message + RESET;
				return colored_message.c_str();
			}
	};

};

#endif
