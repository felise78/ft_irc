#include "IrcBot.hpp"

//Global variable for signal handling
bool signalFlag = false;


IrcBot::IrcBot(const std::string& serverName, int port, const std::string& pass, const std::string& botName)
	: 
	_serverSocket(0),
	_serverPort(port),
	_serverName(serverName),
	_serverPass(pass),
	_botName(botName),
	_serverRequestBuffer("") {

	// Setting up the signal handler
	signal(SIGINT, IrcBot::signalHandler);

	// Initializing the client socket
	initClientSocket();

	// Establishing a connection to the server
	connectToServer();
}

IrcBot::~IrcBot() {
	close(_serverSocket);
}

void IrcBot::initClientSocket() {

	if ((_serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		throw IrcBotException("ERROR: Failure opening socket for IrcBot client.\n");
	}
}

void IrcBot::connectToServer() {

	struct hostent* server = gethostbyname(_serverName.c_str());

	try {
		if (server == NULL) {
			throw IrcBotException("ERROR: no such host [" + _serverName + "]\n");
		}
	}
	catch (IrcBotException& e) {
		std::cerr << e.what();
		exit(1);
	}	

	struct sockaddr_in serv_addr;

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(_serverPort);
	serv_addr.sin_addr = *((struct in_addr*)server->h_addr);

	memset(&(serv_addr.sin_zero), '\0', 8);

	// if (connect(_serverSocket, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
	// 	throw IrcBotException("ERROR: connecting to server failed. Make sure the server is up..\n");
	// 	signalFlag = true;
	// }
	try {
		if (connect(_serverSocket, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
			throw IrcBotException("ERROR: connecting to server failed. Make sure the server is up..\n");
		}
	}	
	catch (IrcBotException& e) {
		std::cerr << e.what();
		exit(1);
	}

	/* DEBUG */
	std::cout << "Connected to server: " << _serverName << " on port: " << _serverPort << std::endl;
	std::cout << "Bot name: " << _botName << std::endl;
	std::cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << std::endl;

	// if (connect(_serverSocket, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
	// 	std::cerr << "error: connecting to server failed\n";
	// }
}

void IrcBot::sendHandshake() {

	sendIrcMessage("PASS " + _serverPass);
	sendIrcMessage("NICK " + _botName);
	sendIrcMessage("USER " + _botName + " 0 * :" + _botName);
}

void IrcBot::joinChannel(const std::string& channel) {

	sendIrcMessage("JOIN " + channel);
}

void IrcBot::sendIrcMessage(const std::string& message) {

	std::string fullMessage = message + "\r\n";

	try {
		if (write(_serverSocket, fullMessage.c_str(), fullMessage.length()) < 0) {
			throw IrcBotException("ERROR: writing to socket failed\n");
		}
	}
	catch (IrcBotException& e) {
		std::cerr << e.what();
		exit(1);
	}
}

void IrcBot::sendMessage(const std::string& channel, const std::string& message) {

	sendIrcMessage("PRIVMSG " + channel + " :" + message);
}

void IrcBot::handleServerRequest() {

	int bytes = 0;
	char		buffer[BUFFER_SIZE];

	memset(buffer, 0, BUFFER_SIZE);

	bytes = read(_serverSocket, buffer, BUFFER_SIZE - 1);
	// if (bytes < 0) {
	// 	throw IrcBotException("ERROR: reading from socket failed\n");
	// }
	// else if (bytes == 0) {
	// 	throw IrcBotException("ERROR: server closed the connection\n");
	// 	signalFlag = true;
	// }

	try {
		if (bytes > 0) {
			buffer[bytes] = '\0';
			_serverRequestBuffer = buffer;
		}
		else if (bytes == 0) {
			throw IrcBotException("ERROR: server closed the connection\n");
		}
		else {
			throw IrcBotException("ERROR: reading from socket failed\n");
		}
	}
	catch (IrcBotException& e) {
		std::cerr << e.what();
		exit(1);
	}

	_serverRequestBuffer = buffer;
}

void IrcBot::handleResponse() {

	std::cout << "Request from the server, RAW: " << _serverRequestBuffer << std::endl;

	std::string response = _serverRequestBuffer;

	if (response.find("PRIVMSG") != std::string::npos) {
		std::string sender = response.substr(1, response.find("!") - 1);
		std::string message = response.substr(response.find("PRIVMSG") + 8);
		std::cout << "Message from " << sender << ": " << message << std::endl;
	}
	else if (response.find("PING") != std::string::npos) {
		sendIrcMessage("PONG " + response.substr(5));
	}
	else if (response.find("DO_THE_THING") != std::string::npos) {
	}
}

void IrcBot::signalHandler(int signal) {
	std::cout << "Interrupt signal (" << signal << ") received. Closing connection and exit.\n";
	signalFlag = true;
	// exit(signal);
}
