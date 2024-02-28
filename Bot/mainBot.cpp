#include "IrcBot.hpp"

/*
** TESTING VALUES (must be passed as arguments to the program)
*/
#define SERVER_NAME "localhost"
#define SERVER_PORT 55555
#define SERVER_PASS "password"
#define BOT_NAME "NeoBot"
#define CHANNEL "#general"


int main(int argc, char	**argv) {

	if (argc != 5) {
		std::cerr << "Error. Wrong number of arguments. Usage: " << std::endl;
		std::cerr << argv[0] << " <SERVER_NAME> <SERVER_PORT> <SERVER_PASSWORD> <BOT_NAME>" << std::endl;
		return 1;
	}

	// Required a bit of parsing of argv to get the server name, port, and bot name
    IrcBot bot(SERVER_NAME, SERVER_PORT, SERVER_PASS, BOT_NAME);

	bot.sendHandshake();

    bot.joinChannel(CHANNEL);
    bot.sendMessage(CHANNEL, "Wake up, Neo... The Matrix has you... ");

    while (!signalFlag) {

        bot.handleServerRequest();
		bot.handleResponse();

    }

    return 0;
}
