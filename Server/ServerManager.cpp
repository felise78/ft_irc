#include "ServerManager.hpp"

ServerManager::ServerManager() {
	// std::cout << MAGENTA << "\tServersManager default constructor called" << RESET << std::endl;

	SM_instance = this; // This is needed for the signal handling

	FD_ZERO(&_recv_fd_pool);
	FD_ZERO(&_send_fd_pool);
	_serverFd = _server.getServerFd();
	_max_fd = _serverFd;
	_sigInt = false;
	_password = "pass";
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

	signal(SIGINT, signalhandler);

	while (!_sigInt) {

		// The copy is needed because select() will modify the fd_sets passed to it
		recv_fd_pool_copy = _recv_fd_pool;
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
**
** ABOUT THE BEHAVIOR OF `CTRL-D` IN THE TERMINAL AND NETCAT: `co^Dmma^Dnd` test
** The terminal and netcat `nc` handle `Ctrl-D` as the EOF (End of File) signal.
** When `Ctrl-D` pressed in the middle of a line (i.e., after typing some characters 
** but before pressing Enter), netcat and the terminal do not interpret this as an EOF signal !
** Instead, they send the current line to the server, and then continue accepting input.
** This is why you can type `co^Dmma^Dnd` and the server shall receive `command`.
**
** However, when `Ctrl-D` pressed on an empty line, netcat and the terminal interpret this as 
** an EOF signal. The connection to the server is then closed, and any subsequent characters 
** are not sent. 
** So, pressind `Ctrl-D` twice or on an empty input, the characters typed after won't go through !!
*/
void	ServerManager::_handle(int fd) {

	// char	buffer[BUF_SIZE] = {0};
	char	buffer[MSG_SIZE] = {0};
	int		bytes_read = 0;

	bytes_read = read(fd, buffer, MSG_SIZE - 1); // -1 to leave space for the null terminator

	/* DEBUG */
	std::cout << timeStamp();
	std::cout << std::endl << MAGENTA << "bytes read:" << bytes_read << " Buffer: " << buffer << std::endl;
	/* ****** */

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

	// UsersMap[fd].requestBuffer.append(buffer, bytes_read);
	usersMap[fd].userMessageBuffer += std::string(buffer, bytes_read);

	/* DEBUG */
	std::cout << std::endl << MAGENTA << "USER MESSAGE BUFFER: " << usersMap[fd].userMessageBuffer << std::endl;
	std::cout << "Size of user msg buffer: " << usersMap[fd].userMessageBuffer.size() << std::endl;
	std::cout << CYAN << "[*] received from client fd[" << fd << "]: " << RESET << std::endl;
	// std::cout << MAGENTA << usersMap[fd].userMessageBuffer << RESET;
	std::cout << CYAN << "parsing..." << RESET << std::endl;
	/* ***** */

	User &user = usersMap[fd];
	if (noCRLFinBuffer(user.userMessageBuffer))
	{
		return ; // if no `\n` found in the buffer, we wait for the next read from this client fd
	}
	if (bytes_read > 1 && usersMap[fd].getPassword().empty() && usersMap[fd].userMessageBuffer.find("PASS") == std::string::npos)
	{
		std::string msg = ERR_PASSWDMISMATCH;
		write(fd, msg.c_str(), msg.size());
		_closeConnection(fd);
		return ;
	}
	vector<string> splitMessageBuffer = split(user.userMessageBuffer, "\n");
	for (vector<string>::iterator it = splitMessageBuffer.begin(); it != splitMessageBuffer.end(); it++)
	{	
		if ((*it).find("PASS ") == 0 || (*it).find("pass ") == 0 || (*it).find("/pass ") == 0 || (*it).find("/PASS ") == 0)
		{
			Request	userRequest(*this, *it);
			map<string, string> input_map = userRequest.getRequestMap();
			CommandHandler cmdHandler(*this, user, input_map);
			splitMessageBuffer.erase(it);
			break ;
		}
	}
	for (vector<string>::iterator it = splitMessageBuffer.begin(); it != splitMessageBuffer.end(); it++)
	{	
		std::cout << MAGENTA << *it << RESET << std::endl;
		Request	userRequest(*this, *it);
		map<string, string> input_map = userRequest.getRequestMap();
		CommandHandler cmdHandler(*this, user, input_map);
	}
	user.userMessageBuffer.clear();

	// We add the client's fd to the send_fd_pool once the client is authenticated (received NICK, USER, PASS..)
	// if (user.authenticated())
	if (!user.responseBuffer.empty()) {
		_removeFromSet(fd, &_recv_fd_pool);
		_addToSet(fd, &_send_fd_pool);
	}

}

/*
** The following function is handling the responses logic from server to the irc client
** `_respond` is called from `run` function once the client's fd is in the send_fd_pool !!
*/
void	ServerManager::_respond(int fd) {

	User &user = usersMap[fd];

	UserResponse	userResponse(user, _server);

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
		/* DEBUG */
		std::cout << GREEN << "[+] Response sent to client fd:[" << fd << "]";
		std::cout << "Response message: " << user.responseBuffer;
		std::cout << ", bytes sent: [" << bytes_sent << "]" << RESET << std::endl;
		std::cout << ". . . . . . . . . . . . . . . . . . . . . . . . . . . " << std::endl;
		// std::cout << CYAN;
		// std::cout << UsersMap[fd].responseBuffer << std::endl;
		// std::cout << RESET;
		/* ***** */
	}

	// if (user.handshaked() == true) {


	// }
	_removeFromSet(fd, &_send_fd_pool);
	_addToSet(fd, &_recv_fd_pool);
	user.userMessageBuffer.clear();
	user.responseBuffer.clear();
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

	if (returnValue == -1 && _sigInt == false) {

		std::cerr << RED << "\t[-]" << msg << RESET << std::endl;
		exit(EXIT_FAILURE);
	}
}

/*
** Do we need this function ?!
*/
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


void	ServerManager::setChannel(const Channel& channel)
{
	if (channelMap.find(channel.getName()) != channelMap.end())
		return;
	channelMap.insert(std::make_pair(channel.getName(), channel));
}

Channel& ServerManager::getChannel( const std::string& name )
{
	return channelMap.at(name);
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

/*UTILS*/

// split splits string input at every occurence of string delimiter, and returns a vector
std::vector<std::string> split(const std::string& input, const std::string& delimiter) 
{
    std::vector<std::string> tokens;
    size_t lastPos = 0;
    size_t pos = input.find(delimiter, lastPos);
    while (pos != std::string::npos)
	{
		string str = input.substr(lastPos, pos - lastPos);
		if (!str.empty()) {
			tokens.push_back(str);}
		lastPos = pos + delimiter.length();
		pos = input.find(delimiter, lastPos);
    }
	string str = input.substr(lastPos);
	if (!str.empty()) {
    	tokens.push_back(str);}
    return tokens;
}

void trim(std::string &str, std::string delimiter)
{
	size_t start = str.find_first_not_of(delimiter);
    if (start != std::string::npos) {
		str = str.substr(start);
    } 
	else {
		str.clear(); // Entire string is whitespace
        return;
    }
    size_t end = str.find_last_not_of(delimiter);
    if (end != std::string::npos) {
        str = str.substr(0, end + 1);
    } else {
        str.clear(); // Entire string is whitespace
    }
}

int	noCRLFinBuffer(std::string const& buffer)
{
	size_t crlf = buffer.find("\n");
	if (crlf == std::string::npos)
		return 1;
	return 0;
}

/*
** This function is called in asituation when a user sends a message to a channel
** i.e. `PRIVMSG #channel :message` where the message needs to be broadcasted to all users in the channel
** In here we just add all the users to `_send_fd_pool` so that select picks them up
** and calls `_respond` to send the message user by user.
** The `responseBuffer` of each user is set to the message to be sent in `Channel::broadcast()`.
*/
void	ServerManager::setBroadcast(std::string msg) {

	std::map<int, User>::iterator it;

	for (it = usersMap.begin(); it != usersMap.end(); it++) {

		setBroadcast(msg, it->first);
	}
}

/*
** This function overloads the previous one and is used to send a message to a specific user
*/
void	ServerManager::setBroadcast(std::string msg, int fd) {

	std::map<int, User>::iterator it = usersMap.find(fd);

	// THE MESSAGE `msg` TO BE SENT MUST BE ALREADY PROPERLY FORMATTED..
	// it->second.responseBuffer = it->second.getPrefix() + " PRIVMSG " + it->second.getChannel() + " :" + msg + "\r\n";
	it->second.responseBuffer += msg;

	_addToSet(fd, &_send_fd_pool);
}

/*
** SIGNAL HANDLING
**
** Initialization of the static member `SM_instance` happens in the constructor above
*/
ServerManager*	ServerManager::SM_instance = NULL;

void	ServerManager::signalhandler(int signal) {

	std::cout << RED << "\t[-] Signal received [" << signal << "] Exiting.." << RESET << std::endl;

	if (SM_instance != NULL) {
		SM_instance->handleSignal();
	}
}

void	ServerManager::handleSignal() {

	// Closing all available connections, cleaning up and terminating the main loop..

	for (int fd = _max_fd; fd >= _serverFd; fd--) {
		_closeConnection(fd);
	}

	_sigInt = true;
}

std::string const&	ServerManager::getPassword() const
{
	return _password;
}