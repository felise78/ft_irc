#ifndef MODE_HANDLER_HPP
#define MODE_HANDLER_HPP

#ifndef DEBUG
	#define DEBUG 0
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
		vector<string>			_flag;
		vector<string>			_extra_args;
		int						n_flags;
		int						n_channels;
		string					_channel;
		vector<string>			_argsEnd;
		bool					_success;

		int		parse_errors();
		void	exec_mode();
		void	handle_flag(string const& flag, Channel &channel, string& add, string& remove);
		void	handle_i(bool set_flag, Channel &channel, string& add, string& remove);
		void	handle_t(bool set_flag, Channel &channel, string& add, string& remove);
		void	handle_k(bool set_flag, Channel &channel, string const& flag, string& add, string& remove);
		void	handle_o(string const& flag, bool set_flag, Channel& channel, string& add, string& remove);
		void	handle_l(bool set_flag, string const& flag, Channel& channel, string& add, string& remove);


	public:
		ModeHandler(map<string, string>& commands, ServerManager& srv, User& user);
		~ModeHandler();
};

#endif