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
	if (port < 1 || port > 65000)
	{
		std::cerr << RED << "Invalid port" << RESET << "\n";
		return 1;
	}
	std::string password = argv[2];
	if (password.empty())
	{
		std::cerr << RED << "Invalid password" << RESET << "\n";
		return 1;
	}
	ServerManager	serverManager(port, password);

	return 0;
}
