#include "ServerManager.hpp"

ServerManager::ServerManager(int port, std::string const& password) : _server(Server(port, password)) {

	SM_instance = this; // This is needed for the signal handling
	hostname = _server.getServerHostName();
	FD_ZERO(&_recv_fd_pool);
	FD_ZERO(&_send_fd_pool);
	_serverFd = _server.getServerFd();
	_max_fd = _serverFd;
	_sigInt = false;
	_server.printServerData(); // This prints the server's data (hostname, port, password..)
	_fcntl(); // This will set the server socket fd to non-blocking mode and add it to the recv_fd_pool
	_run(); // This will start the main loop of the server
}

ServerManager::~ServerManager() {
}

/*
** Main loop of the server is here (accepting new connections, handling requests, responding to clients..)
*/
void	ServerManager::_run() {

	fd_set	recv_fd_pool_copy;
	fd_set	send_fd_pool_copy;
	int		select_ret = 0;

	signal(SIGINT, signalhandler);

	while (!_sigInt) {
		
		recv_fd_pool_copy = _recv_fd_pool; // ..the copy is needed because select() will modify the fd_sets passed to it
		send_fd_pool_copy = _send_fd_pool;

		select_ret = select(_max_fd + 1, &recv_fd_pool_copy, &send_fd_pool_copy, NULL, NULL);
		checkErrorAndExit(select_ret, "select() failed. Exiting..");

		for (int fd = 3; fd <= _max_fd && _sigInt == false; fd++) {

			if (FD_ISSET(fd, &recv_fd_pool_copy) && fd == _server.getServerFd()) {
				_accept(fd);
			}
			if (FD_ISSET(fd, &recv_fd_pool_copy) && isClient(fd)) {
				_handle(fd);
			}
			if (FD_ISSET(fd, &send_fd_pool_copy)) {
				_respond(fd);
			}
		}
	}
}

/*
** As long as `select()` returns a positive value
** we call `accept()` to accept the new connection to the server.
** We also add the new client's fd to the recv_fd_pool to be handled later in `_handle()`. 
*/
void	ServerManager::_accept(int clientFd) {

	struct sockaddr_in	address;
	socklen_t			address_len = sizeof(address);

	clientFd = accept(_serverFd, (struct sockaddr *)&address, (socklen_t *)&address_len);

	if (clientFd == -1) {
		std::cerr << RED << "\t[-] Error accepting connection.. accept() failed..";
		std::cerr << " serverFd: [" << _serverFd << "], clientFd:[" << clientFd << "]" << std::endl;
		std::cerr << RESET << std::endl;
		return ;
	}
	std::cout << timeStamp() << GREEN << "[+] New connection. Cliend fd: [" << clientFd << "], IP:[" << inet_ntoa(address.sin_addr) << "], port:[" << ntohs(address.sin_port) << "]" << RESET << std::endl;

	_addToSet(clientFd, &_recv_fd_pool);

	// This will set the client socket fd to non-blocking mode (needed for select(), read(), recv(), write(), send()..)
	int return_value = fcntl(clientFd, F_SETFL, O_NONBLOCK);
	if (return_value == -1) {
		std::cerr << RED << "\t[-] Error setting socket to non-blocking mode.. fcntl() failed." << RESET << std::endl;
		_closeConnection(clientFd);
		return ;
	}

	int	enable = 1;
	return_value = setsockopt(clientFd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));
	checkErrorAndExit(return_value, "\t[-] Error setting socket options.. setsockopt() failed.");

	initUser(clientFd, address); // Initializing the new User and adding its class instance to the UsersMap
}

/*
** This is handling the requests from the irc client side
** `_handle` is called from `run` function once the new connection 
** is accepted and the client's fd is added to the recv_fd_pool !! 
*/
void	ServerManager::_handle(int fd) {

	char	buffer[MSG_SIZE] = {0};
	int		bytes_read = 0;

	bytes_read = read(fd, buffer, MSG_SIZE - 1); // -1 to leave space for the null terminator

	if (bytes_read == 0) {
		std::cout << YELLOW << "[!] bytes_read == 0 from client fd:[" << fd << "]" << RESET << std::endl;
		_closeConnection(fd);
		return ;
	}
	else if (bytes_read < 0) {
		std::cerr << RED << "[-] Error reading data from the client.. read() failed." << RESET << std::endl;
		_closeConnection(fd);
		return ;
	}

	// Processing the input message from the client
	User &user = usersMap[fd];
	user.userMessageBuffer += std::string(buffer, bytes_read);

	if (noCRLFinBuffer(user.userMessageBuffer))
		return ; // if no `\n` found in the buffer, we wait for the next read from this client fd

	vector<string> splitMessageBuffer = split(user.userMessageBuffer, "\n"); // Vector is used to split the input message buffer by `\n`.. this way one string in this vector is a command with its parameters
	for (size_t i = 0; i < splitMessageBuffer.size(); i++) {

		std::cout << MAGENTA << splitMessageBuffer[i] << RESET << std::endl;
		Request	userRequest(*this, splitMessageBuffer[i]);
		map<string, string> input_map = userRequest.getRequestMap();
		CommandHandler cmdHandler(*this, user, input_map);
	}
	user.userMessageBuffer.clear();
}

/*
** The following function is handling the responses logic from server to the irc client
** `_respond` is called from `run` function once the client's fd is in the send_fd_pool !!
*/
void	ServerManager::_respond(int fd) {

	User &user = usersMap[fd];

	int		bytes_sent = 0;
	int		bytes_to_send = user.responseBuffer.length();

	bytes_sent = send(fd, user.responseBuffer.c_str(), bytes_to_send, 0);
	std::cout << timeStamp();

	if (bytes_sent == -1) {
		std::cerr << RED << "[-] Error sending data to the User.. send() failed." << RESET << std::endl;
		_closeConnection(fd);
		return ;
	}

	_removeFromSet(fd, &_send_fd_pool);
	_addToSet(fd, &_recv_fd_pool);

	user.userMessageBuffer.clear();
	user.responseBuffer.clear();

	if (user.getStatus() == DELETED) {
		std::cout << RED << "[-] User Deleted. Closing connection, fd:[" << fd << "]" << RESET << std::endl;
		_closeConnection(fd);
	}
}

/*
** This function sets the server's socket fd to non-blocking mode
*/
void	ServerManager::_fcntl() {

	int	fcntl_ret = fcntl(_serverFd, F_SETFL, O_NONBLOCK);
	checkErrorAndExit(fcntl_ret, "fcntl() failed. Exiting..");

	_addToSet(_serverFd, &_recv_fd_pool);
}

// Adds file descriptor to a set
void	ServerManager::_addToSet(int fd, fd_set *set) {

	if (fd > _max_fd) {
		_max_fd = fd;
	}
	FD_SET(fd, set);
}

void	ServerManager::_removeFromSet(int fd, fd_set *set) {

	if (fd == _max_fd) {
		_max_fd--;
	}
	FD_CLR(fd, set);
}

void	ServerManager::_closeConnection(int fd) {
	std::cout << timeStamp() << YELLOW << "[!] Closing connection with fd:[" << fd << "]." << RESET << std::endl;

	if (FD_ISSET(fd, &_recv_fd_pool)) {
		_removeFromSet(fd, &_recv_fd_pool);
	}
	if (FD_ISSET(fd, &_send_fd_pool)) {
		_removeFromSet(fd, &_send_fd_pool);
	}
	close(fd);
	usersMap.erase(fd);
}

/*
** This function will initialize an instance of a user
** as well as add its to the UsersMap !!
*/
void	ServerManager::initUser(int clientFd, struct sockaddr_in &address) {

	User	newUser;

	char	*client_ip = inet_ntoa(address.sin_addr);
	if (client_ip == NULL) {
		std::cerr << RED << "\t[-] Error: inet_ntoa() failed." << RESET << std::endl;
		exit(EXIT_FAILURE);
	}

	int	port = ntohs(address.sin_port);
	newUser.setPort(port);
	newUser.setFd(clientFd);
	newUser.setHostName(client_ip);

	usersMap.insert(std::make_pair(clientFd, newUser));

	std::cout << timeStamp() << GREEN << "[+] New connection.. fd:[" << clientFd << "], IP:[" << client_ip << "], port:[" << port << "]" << RESET << std::endl;
}


/*
** GENERAL HELPER FUNCTIONS
*/
std::string	ServerManager::timeStamp() {

	std::time_t currentTime = std::time(NULL);
    std::tm* now = std::localtime(&currentTime);
    char buffer[64];
    std::strftime(buffer, sizeof(buffer), "[%d/%m/%Y %H:%M:%S]", now);

	return std::string(buffer);
}

void	ServerManager::checkErrorAndExit(int returnValue, const std::string& msg) {

	if (returnValue == -1 && _sigInt == false) {

		std::cerr << RED << "\t[-]" << msg << RESET << std::endl;
		exit(EXIT_FAILURE);
	}
}

bool	ServerManager::isClient(int fd) {

	return usersMap.count(fd) > 0;
}


void	ServerManager::setChannel(const Channel& channel) {

	if (channelMap.find(channel.getName()) != channelMap.end()) {
		return;
	}
	channelMap.insert(std::make_pair(channel.getName(), channel));
}

Channel& ServerManager::getChannel( const std::string& name ) {

	return channelMap.at(name);
}

int ServerManager::getFdbyNickName( const std::string& nickname ) const {

	std::map<int, User>::const_iterator it;
	for (it = usersMap.begin(); it != usersMap.end(); ++it) {
		if (it->second.getNickName() == nickname)
			return it->second.getFd();
	}
	return -1;
}

/*
** This function is called in asituation when a user sends a message to a channel
** i.e. `PRIVMSG #channel :message` where the message needs to be broadcasted to all users in the channel
** In here we just add all the users to `_send_fd_pool` so that select picks them up
** and calls `_respond` to send the message user by user.
** The `responseBuffer` of each user is set to the message to be sent in `Channel::broadcast()`.
*/
void	ServerManager::setBroadcast(std::string channelName, std::string sender_nick, std::string msg) {

	std::map<string, User* >::iterator it = channelMap[channelName]._users.begin();

	for ( ; it != channelMap[channelName]._users.end(); it++) {
		if (it->second->getNickName() != sender_nick)
			setBroadcast(msg, it->second->getFd());
	}
}

/*
** This function overloads the previous one and is used to send a message to a specific user
*/
void	ServerManager::setBroadcast(std::string msg, int fd) {

	std::map<int, User>::iterator it = usersMap.find(fd);
	if (it == usersMap.end())
		return ;
	if (it != usersMap.end()) {
		it->second.responseBuffer += msg;
	}
	_removeFromSet(fd, &_recv_fd_pool);
	_addToSet(fd, &_send_fd_pool);
}

/*
** SIGNAL HANDLING
** Initialization of the static member `SM_instance` happens in the constructor above
*/
ServerManager*	ServerManager::SM_instance = NULL;

void	ServerManager::signalhandler(int signal) {
	(void)signal;

	if (SM_instance != NULL) {
		SM_instance->handleSignal();
	}
}

void	ServerManager::handleSignal() {
	std::cout << RED << "\t[-] SIGINT received. Shutting down the server. Bye.." << RESET << std::endl;

	for (int fd = _max_fd; fd > _serverFd; fd--) {
		_closeConnection(fd);
	}
	_sigInt = true;
}

std::string const&	ServerManager::getPassword() const {
	return (_server.getServerPassword());
}
