#include "ModeHandler.hpp"

/* CONSTRUCTOR/DESTRUCTOR */
ModeHandler::ModeHandler(map<string, string>& commands, ServerManager& srv, User& user) : _commandsFromClient(commands), _server(srv), _user(user), n_flags(0), n_channels(0)
{
	if (_user.getStatus() != REGISTERED)
	{
		// srv.setBroadcast(ERR_NOTREGISTERED, _user.getSocket());
		srv.setBroadcast(ERR_NOTREGISTERED(_server.hostname), _user.getSocket());
		return ;
	}
	if (parse_errors() != 0)
		return ;
	exec_mode();	
}

ModeHandler::~ModeHandler()
{
}

int	ModeHandler::parse_errors()
{
	if (_commandsFromClient["params"].find("#") == std::string::npos && _commandsFromClient["params"].find("&") == std::string::npos)
		return 1;
	stringstream params;
	params.str(_commandsFromClient["params"]);
	vector<string> args;
	string			tmp;
	while (getline(params, tmp, ' '))
	{
		if (!tmp.empty())
			args.push_back(tmp);
	}
	for (size_t i = 0; i < args.size(); i++)
	{
		if (args[i][0] == '#' || args[i][0] == '&')
		{
			n_channels++;
			if (_server.channelMap.find(args[i]) != _server.channelMap.end())
				_channel = args[i];
			else
			{
				// _server.setBroadcast(ERR_NOSUCHCHANNEL(args[i]), _user.getSocket());
				_server.setBroadcast(ERR_NOSUCHCHANNEL(_server.hostname, args[i]), _user.getSocket());
				return 1;
			}
		}
		if (args[i][0] == '+' || args[i][0] == '-')
		{
			_flag = args[i];
			for (size_t i = 1; i < _flag.size(); i++)
			{
				const string modes = "itkol";
				if (_flag.size() < 2 || modes.find(_flag[i]) == string::npos)
				{
					// _server.setBroadcast(ERR_UMODEUNKNOWNFLAG(args[i]), _user.getSocket());
					_server.setBroadcast(ERR_UMODEUNKNOWNFLAG(_server.hostname, args[i]), _user.getSocket());
					return 1;
				}
			}
			n_flags++;
		}
		if (i > 2)
		{
			_extra_args.push_back(args[i]);
		}
	}
	if (n_flags < 1 || n_channels < 1)
	{
		// _server.setBroadcast(ERR_NEEDMOREPARAMS(_commandsFromClient["command"]), _user.getSocket());
		_server.setBroadcast(ERR_NEEDMOREPARAMS(_server.hostname, _commandsFromClient["command"]), _user.getSocket());
		return 1;
	}
	if (_extra_args.size() > 1 || n_flags > 1 || n_channels > 1)
	{
		// _server.setBroadcast(ERR_TOOMANYTARGETS(_commandsFromClient["command"]), _user.getSocket());
		_server.setBroadcast(ERR_TOOMANYTARGETS(_server.hostname, _commandsFromClient["command"]), _user.getSocket());
		return 1;
	}
	Channel &c_tmp = _server.channelMap[_channel];
	std::string nickname = _user.getNickName();
	if (c_tmp.isOp(nickname) == true)
		return 0;
	else
	{
		// _server.setBroadcast(ERR_CHANOPRIVSNEEDED(_channel), _user.getSocket());
		_server.setBroadcast(ERR_CHANOPRIVSNEEDED(_server.hostname, _channel), _user.getSocket());
		return 1;
	}
}

void	ModeHandler::exec_mode()
{
	bool	set_flag;
	std::map<std::string, Channel>::iterator it = _server.channelMap.find(_channel);
	if (it == _server.channelMap.end() || _flag.empty())
		return ;
	Channel &channel = it->second;
	if (!(_flag.empty()) && _flag[0] == '+')
		set_flag = true;
	if (!(_flag.empty()) && _flag[0] == '-')
		set_flag = false;	
	std::cout << "Flag is " << _flag << ".\n";
	for (size_t i = 1; i < _flag.size(); i++)
	{
		if (_flag[i] == 'i')
			channel.setInvit(set_flag);
		if (_flag[i] == 't')
			channel.setTopicRestricted(set_flag);
		if (_flag[i] == 'k')
		{
			channel.setProtected(set_flag);
			if (!_extra_args.empty())
				channel.setKey(_extra_args[0]);
		}
		if (_flag[i] == 'o')
		{
			if (_extra_args.empty())
			{
				std::string cmd = "MODE";
				// _server.setBroadcast(ERR_NEEDMOREPARAMS(cmd), _user.getSocket());
				_server.setBroadcast(ERR_NEEDMOREPARAMS(_server.hostname, cmd), _user.getSocket());
				return ;
			}
			else if (channel._users.find(_extra_args[0]) == channel._users.end())
			{
				// _server.setBroadcast(ERR_NOTONCHANNEL(_channel), _user.getSocket());
				_server.setBroadcast(ERR_NOTONCHANNEL(_server.hostname, _channel), _user.getSocket());
				return ;
			}
			else {
				if (set_flag)
					channel.setOp(_extra_args[0]);
				else
					channel.removeOp(_extra_args[0]);
			}
		}
		if (_flag[i] == 'l')
		{
			if (set_flag)
				channel.setLimit(atoi(_extra_args[0].c_str()));
			else
				channel.removeLimit();
		}
	}
	string msg = _user.getPrefix() + " " + _user.userMessageBuffer;
	_server.setBroadcast(msg, _user.getSocket());
	_server.setBroadcast(_channel, _user.getNickName(), msg);
}

