// #include "Sockets/HeaderSockets.hpp"
#include "Server/ServerManager.hpp"


int	main(int argc, char **argv) {

	if (argc != 3)
	{
		std::cerr << RED << "Usage: " << RESET << "./ircserv <port> <password>\n";
		return 1;
	}
	int port = atoi(argv[1]); // Probably needs more error handling here. 
							// Which ports are valid? Eg. Tried 0, and server launches, but connections from irssi impossible.
	std::string password = argv[2];
	ServerManager	serverManager(port, password);

	return 0;
}
