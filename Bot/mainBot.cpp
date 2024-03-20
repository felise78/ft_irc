#include "IrcBot.hpp"

/*
** TESTING VALUES (must be passed as arguments to the program)
*/
// #define SERVER_NAME "localhost"
// #define SERVER_NAME "127.0.0.1"
// #define SERVER_PORT 55555
// #define SERVER_PASS "password"

// Defined manually for a reason (must be unique to identify it on the server side)
#define BOT_NAME "NeoBot"
#define CHANNEL "#helpdesk"

typedef struct s_server_data {

	std::string	serverName;
	std::string	serverPass;
	int			serverPort;

}				t_server_data;

bool parseArguments(int argc, char **argv, t_server_data &serverData) {

	if (argc != 4) {
		std::cerr << RED << "Error. Wrong number of arguments. Usage: " << RESET << std::endl;
		std::cerr << MAGENTA << argv[0] << " <SERVER_NAME> <SERVER_PORT> <SERVER_PASSWORD>" << RESET << std::endl;
		return false;
	}

	serverData.serverName = argv[1];
	serverData.serverPass = argv[3];	

	// Check if the port is a number and in the range of 1024 - 65535
	std::string portStr = argv[2];
	std::stringstream ss(portStr);
	int port;
	ss >> port;

	if (port < 1024 || port > 65535) {
		std::cerr << "Error. The port number must be in the range of 1024 - 65535." << std::endl;
		return false;
	}
	serverData.serverPort = port;

	return true;
}


int main(int argc, char	**argv) {

	t_server_data serverData;

	if (parseArguments(argc, argv, serverData) == false) {
		return 1;
	}

    // IrcBot bot(SERVER_NAME, SERVER_PORT, SERVER_PASS, BOT_NAME);
	IrcBot bot(serverData.serverName, serverData.serverPort, serverData.serverPass, BOT_NAME);

    while (bot.signalErrorFlag == false) {
		
		// if the bot is registered/authenticated then it joins the channel and sends initial message
		if (!bot._authenticated) {
			bot.sendHandshake();
		}

        bot.handleServerRequest();
		
		if (bot.signalErrorFlag == false) {
			bot.handleResponse();
		}

    }

	std::cout << GREEN << "Bye.." << RESET << std::endl;

    return 0;
}
