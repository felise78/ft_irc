#include "ModeHandler.hpp"

/* CONSTRUCTOR/DESTRUCTOR */
ModeHandler::ModeHandler(map<string, string>& commands, ServerManager& srv, User& user) : _commandsFromClient(commands), _server(srv), _user(user), n_flags(0), n_channels(0)
{
	if (parse_errors() != 0)
		return ;
	exec_mode();	
}

ModeHandler::~ModeHandler()
{
}

int	ModeHandler::parse_errors()
{
	stringstream params;
	params.str(_commandsFromClient["params"]);
	std::cout << params << std::endl;
	vector<string> args;
	string			tmp;
	while (getline(params, tmp, ' '))
	{
		if (!tmp.empty())
			args.push_back(tmp);
	}
	typedef vector<string>::iterator stringVecIt;

	stringVecIt it = args.begin();
	for (; it != args.end(); it++)
	{
		if ((*it)[0] == '#' || (*it)[0] == '&')
		{
			n_channels++;
			if (_server.channelMap.find(*it) != _server.channelMap.end())
				_channel = *it;
			else
			{
				_server.setBroadcast(ERR_NOSUCHCHANNEL(*it), _user.getSocket());
				return 1;
			}
		}
		else if ((*it)[0] == '+' || (*it)[0] == '-')
		{
			_flag = *it;
			for (size_t i = 1; i < _flag.size(); i++)
			{
				const string modes = "itkol";
				if (modes.find(_flag[i]) == string::npos)
				{
					_server.setBroadcast(ERR_UMODEUNKNOWNFLAG(*it), _user.getSocket());
					return 1;
				}
			}
			n_flags++;
		}
		else
		{
			_extra_args.push_back(*it);
		}
	}
	if (n_channels < 1)
		return 1;
	if (n_flags != 1 || n_channels != 1)
	{
		_server.setBroadcast(ERR_NEEDMOREPARAMS(_commandsFromClient["command"]), _user.getSocket());
		return 1;
	}
	if (_extra_args.size() > 1)
	{
		_server.setBroadcast(ERR_TOOMANYTARGETS(_commandsFromClient["command"]), _user.getSocket());
		return 1;
	}
	Channel &c_tmp = _server.channelMap[_channel];
	if (c_tmp.isOp(_user.getNickName()))
		return 0;
	else
	{
		_server.setBroadcast(ERR_CHANOPRIVSNEEDED(_channel), _user.getSocket());
		return 1;
	}
}

void	ModeHandler::exec_mode()
{
	bool	set_flag;
	std::map<std::string, Channel>::iterator it = _server.channelMap.find(_channel);
	Channel &channel = it->second;
	if (!(_flag.empty()) && _flag[0] == '+')
		set_flag = true;
	if (!(_flag.empty()) && _flag[0] == '-')
		set_flag = false;
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
			if (set_flag)
				channel.setOp(_extra_args[0]);
			else
				channel.removeOp(_extra_args[0]);
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
}

