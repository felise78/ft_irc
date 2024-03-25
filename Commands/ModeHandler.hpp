#ifndef MODE_HANDLER_HPP
#define MODE_HANDLER_HPP

#ifndef DEBUG
	#define DEBUG 1
#endif

#include <string>
#include <map>
#include "../Channel/Channel.hpp"
#include "../Server/ServerManager.hpp"
#include "../User/User.hpp"

using namespace std;

class Channel;
class ServerManager;
class User;

class ModeHandler
{
	private:
		map<string, string>&	_commandsFromClient;
		ServerManager&			_server;
		User&					_user;
		string					_flag;
		vector<string>			_extra_args;
		int						n_flags;
		int						n_channels;
		string					_channel;

		int		parse_errors();
		void	exec_mode();

	public:
		ModeHandler(map<string, string>& commands, ServerManager& srv, User& user);
		~ModeHandler();
};

#endif