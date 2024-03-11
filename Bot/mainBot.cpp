#include "IrcBot.hpp"

/*
** TESTING VALUES (must be passed as arguments to the program)
*/
// #define SERVER_NAME "localhost"
#define SERVER_NAME "127.0.0.1"
#define SERVER_PORT 55555
#define SERVER_PASS "password"

// Defined manually for a reason (must be unique to identify it on the server side)
#define BOT_NAME "NeoBot"
#define CHANNEL "#helpdesk"


int main(int argc, char	**argv) {

	if (argc != 4) {
		std::cerr << "Error. Wrong number of arguments. Usage: " << std::endl;
		std::cerr << argv[0] << " <SERVER_NAME> <SERVER_PORT> <SERVER_PASSWORD>" << std::endl;
		return 1;
	}

	// Required a bit of parsing of argv to get the server name, port, and bot name
    IrcBot bot(SERVER_NAME, SERVER_PORT, SERVER_PASS, BOT_NAME);

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

	std::cout << YELLOW << "THE END..." << RESET << std::endl;

    return 0;
}
