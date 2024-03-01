#ifndef MODE_HANDLER_HPP
#define MODE_HANDLER_HPP

#ifndef DEBUG
	#define DEBUG 0
#endif

#include <string>
#include <map>
#include "../Channel/Channel.hpp"
#include "../Server/ServerManager.hpp"

using namespace std;

class Channel;
class ServerManager;

class ModeHandler
{
	private:
		map<string, string>&	_commandsFromClient;
		ServerManager&			_server;
		int						n_flags;
		int						n_channels;
		Channel*				_channel;
		bool					_i;
		bool					_t;
		bool					_k;
		bool					_o;
		bool					_l;

		int		parse_errors();
		void	exec_mode();

	public:
		ModeHandler(map<string, string>& commands, ServerManager& srv);
		~ModeHandler();
};

#endif