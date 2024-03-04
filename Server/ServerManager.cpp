#include "ServerManager.hpp"

ServerManager::ServerManager() {
	// std::cout << MAGENTA << "\tServersManager default constructor called" << RESET << std::endl;

	FD_ZERO(&_recv_fd_pool);
	FD_ZERO(&_send_fd_pool);
	_serverFd = _server.getServerFd();
	_max_fd = _serverFd;

	// DEBUG PRINT SERVERS DATA
	_server.printServerData();

	// This will set the server socket fd to non-blocking mode and add it to the recv_fd_pool
	_fcntl();

	// This will start the main loop of the server
	_run();
}

ServerManager::~ServerManager() {
	// std::cout << RED << "\tServersManager destructor called" << RESET << std::endl;
}

/*
** Main loop of the server is here (accepting new connections, handling requests, responding to clients..)
*/
void	ServerManager::_run() {

	fd_set	recv_fd_pool_copy;
	fd_set	send_fd_pool_copy;
	int		select_ret = 0;

	while (true) {

		// The copy is needed because select() will modify the fd_sets passed to it
		recv_fd_pool_copy = _recv_fd_pool;
		send_fd_pool_copy = _send_fd_pool;

		select_ret = select(_max_fd + 1, &recv_fd_pool_copy, &send_fd_pool_copy, NULL, NULL);
		checkErrorAndExit(select_ret, "select() failed. Exiting..");

		for (int fd = 3; fd <= _max_fd; fd++) {

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

		// check for timeout ?!
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

	// Initializing the new User and adding its class instance to the UsersMap
	initUser(clientFd, address);

	log(clientFd);
}

/*
** This is handling the requests from the irc client side
** `_handle` is called from `run` function once the new connection 
** is accepted and the client's fd is added to the recv_fd_pool !! 
*/
void	ServerManager::_handle(int fd) {

	char	buffer[BUF_SIZE] = {0};
	int		bytes_read = 0;

	bytes_read = read(fd, buffer, BUF_SIZE);
	std::cout << timeStamp();

	if (bytes_read == 0) {
		std::cout << YELLOW << "[!] bytes_read == 0 from client fd:[" << fd << "]" << RESET << std::endl;
		//std::cout << YELLOW << "[!] Connection closed by the client. ";
		_closeConnection(fd);
		return ;
	}
	else if (bytes_read < 0) {
		std::cerr << RED << "[-] Error reading data from the client.. read() failed." << RESET << std::endl;
		_closeConnection(fd);
		return ;
	}

	// UsersMap[fd].requestBuffer.append(buffer, bytes_read);
	usersMap[fd].userMessageBuffer = std::string(buffer, bytes_read);

	/* DEBUG */
	// At this point received data can be parsed and added to User's class (NAME, NICK, PASS etc..)
	std::cout << CYAN << "[*] received from client fd[" << fd << "]: " << RESET << std::endl;
	// std::cout << MAGENTA << usersMap[fd].userMessageBuffer << RESET;
	std::cout << CYAN << "parsing..." << RESET << std::endl;

	User &user = usersMap[fd];

	// UserRequest	userRequest(user.userMessageBuffer);
	UserRequestParsing	userRequest(*this, user);
	// userRequest.printCommands();
	/* ***** */

	// We add the client's fd to the send_fd_pool once the client is authenticated (received NICK, USER, PASS..)
	if (user.authenticated()) {
		_removeFromSet(fd, &_recv_fd_pool);
		_addToSet(fd, &_send_fd_pool);
	}
	/* ***** */

	// _removeFromSet(fd, &_recv_fd_pool);
	// _addToSet(fd, &_send_fd_pool);
}

/*
** The following function is handling the responses logic from server to the irc client
** `_respond` is called from `run` function once the client's fd is in the send_fd_pool !!
*/
void	ServerManager::_respond(int fd) {

	User &user = usersMap[fd];

	UserResponse	userResponse(user, _server);

	/* DEBUG */
	// According to the protocol the first response to the IRC client shall be 001 RPL_WELCOME, 002 RPL_YOURHOST, 003 RPL_CREATED, 004 RPL_MYINFO
	// Once received data from the client is complete the status of the User can be changed to `READY` and the first response can be composed and sent
	// composeResponse(fd);	
	/* ***** */

	int		bytes_sent = 0;
	int		bytes_to_send = user.responseBuffer.length();

	bytes_sent = send(fd, user.responseBuffer.c_str(), bytes_to_send, 0);

	std::cout << timeStamp();

	if (bytes_sent == -1) {
		std::cerr << RED << "[-] Error sending data to the User.. send() failed." << RESET << std::endl;
		_closeConnection(fd);
		return ;
	}
	else {
		std::cout << GREEN << "[+] Response sent to client fd:[" << fd << "]";
		std::cout << ", bytes sent: [" << bytes_sent << "]" << RESET << std::endl;
		std::cout << ". . . . . . . . . . . . . . . . . . . . . . . . . . . " << std::endl;
	}

	/* DEBUG */
	// std::cout << CYAN;
	// std::cout << UsersMap[fd].responseBuffer << std::endl;
	// std::cout << RESET;
	/* ***** */

	if (user.handshaked() == true) {

		_removeFromSet(fd, &_send_fd_pool);
		_addToSet(fd, &_recv_fd_pool);

		user.userMessageBuffer.clear();
		user.responseBuffer.clear();
	}
}


/*
** This function sets the server's socket fd to non-blocking mode
*/
void	ServerManager::_fcntl() {

	int	fcntl_ret = 0;

	fcntl_ret = fcntl(_serverFd, F_SETFL, O_NONBLOCK);
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
** 
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

	newUser.setSocket(clientFd);
	newUser.setHostName(client_ip);

	// newUser.setHostName("must be parsed from the User's request: `UserMessageBuffer`");
	// newUser.setUserName("must be parsed from the User's request: `UserMessageBuffer`");
	// newUser.setPassword("must be parsed from the User's request: `UserMessageBuffer`");

	usersMap.insert(std::make_pair(clientFd, newUser));
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

	if (returnValue == -1) {

		std::cerr << RED << "\t[-]" << msg << RESET << std::endl;
		exit(EXIT_FAILURE);
	}
}

void	ServerManager::log(int clientFd) {

	User &user = usersMap[clientFd];
	(void)user;

	// std::cout << timeStamp() << YELLOW << "[!] Logging client fd:[" << user.getSocket() << "]" << RESET << std::endl;
	// std::cout << YELLOW << "\tport: " << user.getPort() << RESET << std::endl;
	// std::cout << YELLOW << "\thostName: " << user.getHostName() << RESET << std::endl;
	// std::cout << YELLOW << "\tnickName: " << User.getNickName() << RESET << std::endl;
	// std::cout << YELLOW << "\tuserName: " << User.getUserName() << RESET << std::endl;
	// std::cout << YELLOW << "\tpassword: " << User.getPassword << RESET << std::endl;
	// std::cout << YELLOW << "\trequestBuffer: " << User.UserMessageBuffer << RESET << std::endl;
	// std::cout << YELLOW << "\tresponseBuffer: " << User.responseBuffer << RESET << std::endl;

}

bool	ServerManager::isClient(int fd) {

	// return (UsersMap.find(fd) != UsersMap.end())
	return usersMap.count(fd) > 0;
}


void	ServerManager::setChannel(Channel& channel)
{
	if (channelMap.find(channel.getName()) != channelMap.end())
		return;
	channelMap[channel.getName()] = &channel;
}

Channel& ServerManager::getChannel( const std::string& name ) const
{
	return *channelMap.at(name);
}

int ServerManager::getFdbyNickName( const std::string& nickname ) const
{
	std::map<int, User>::const_iterator it;

	for (it = usersMap.begin(); it != usersMap.end(); ++it)
	{
		if (it->second.getNickName() == nickname)
			return it->second.getSocket();
	}
	return -1;
}