#include "ModeHandler.hpp"

/* CONSTRUCTOR/DESTRUCTOR */
ModeHandler::ModeHandler(map<string, string>& commands, ServerManager& srv, User& user) : _commandsFromClient(commands), _server(srv), _user(user), n_flags(0), n_channels(0)
{
	if (_user.getStatus() != REGISTERED)
	{
		srv.setBroadcast(ERR_NOTREGISTERED, _user.getSocket());
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
	// DEBUG //
	std::cout << "params : " << params.str() << std::endl;
	while (getline(params, tmp, ' '))
	{
		if (!tmp.empty())
			args.push_back(tmp);
	}
	// DEBUG //
	for (size_t i = 0; i < args.size(); i++)
		std::cout << "arg[" << i << "] : " << args[i] << " ";
	std::cout << std::endl;
	// 
	for (size_t i = 0; i < args.size(); i++)
	{
		if (args[i][0] == '#' || args[i][0] == '&')
		{
			n_channels++;
			if (_server.channelMap.find(args[i]) != _server.channelMap.end())
				_channel = args[i];
			else
			{
				_server.setBroadcast(ERR_NOSUCHCHANNEL(args[i]), _user.getSocket());
				return 1;
			}
		}
		else if (args[i][0] == '+' || args[i][0] == '-')
		{
			if (i != 1)
			{
				_server.setBroadcast(ERR_UMODEUNKNOWNFLAG(args[i]), _user.getSocket());
				return 1;
			}
			_flag = args[i];
			for (size_t i = 1; i < _flag.size(); i++)
			{
				const string modes = "itkol";
				if (_flag.size() < 2 || modes.find(_flag[i]) == string::npos)
				{
					_server.setBroadcast(ERR_UMODEUNKNOWNFLAG(args[i]), _user.getSocket());
					return 1;
				}
			}
			n_flags++;
		}
		else
		{
			
			_extra_args.push_back(args[i]);
		}
	}
	// DEBUG //
	std::cout << "nb chan : " << n_channels << std::endl;
	// 
	if (n_flags < 1 || n_channels < 1)
	{
		_server.setBroadcast(ERR_NEEDMOREPARAMS(_commandsFromClient["command"]), _user.getSocket());
		return 1;
	}
	if (_extra_args.size() > 1 || n_flags > 1 || n_channels > 1)
	{
		_server.setBroadcast(ERR_TOOMANYTARGETS(_commandsFromClient["command"]), _user.getSocket());
		return 1;
	}
	Channel &c_tmp = _server.channelMap[_channel];
	std::string nickname = _user.getNickName();
	if (c_tmp.isOp(nickname) == true)
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
	if (it == _server.channelMap.end() || _flag.empty())
		return ;
	Channel &channel = it->second;
	if (!(_flag.empty()) && _flag[0] == '+')
		set_flag = true;
	if (!(_flag.empty()) && _flag[0] == '-')
		set_flag = false;
	// DEBUG //
	std::cout << "Flag is " << _flag << ".\n";
	//
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
		/*
			If a user attempts to make themselves an operator using the "+o"
   			flag, the attempt should be ignored.  There is no restriction,
   			however, on anyone `deopping' themselves (using "-o").
		*/
		if (_flag[i] == 'o')
		{
			// DEBUG //
			std::cout << MAGENTA << "MODE 'o'" << RESET << std::endl;
			for (size_t i = 0; i < _extra_args.size(); i++)
			std::cout << "_extra_args[" << i << "] : " << _extra_args[i] << " ";
			std::cout << std::endl;
			// 
			if (_extra_args.size() < 2)
			{
				// DEBUG //
				//std::cout << "rentre ici ? \n";
				std::string cmd = "MODE";
				_server.setBroadcast(ERR_NEEDMOREPARAMS(cmd), _user.getSocket());
				return ;
			}
			else if (_server.usersMap.find(_server.getFdbyNickName(_extra_args[0])) == _server.usersMap.end())
			{
				_server.setBroadcast(ERR_NOSUCHNICK(_extra_args[0]), _user.getSocket());
				return;
			}
			else if (channel._users.find(_extra_args[0]) == channel._users.end())
			{
				// DEBUG //
				// std::cout << "ne rentre pas ici ? \n";
				_server.setBroadcast(ERR_USERNOTINCHANNEL(_extra_args[0],_channel), _user.getSocket());
				return ;
			}
			else
			{
				if (set_flag)
				{
					if (channel.isOp(_extra_args[0]) == true)
						{;} // ignored if is already op 
					channel.setOp(_extra_args[0]);
					_server.setBroadcast(MODE_USERMSG(_extra_args[0], "+o"), _server.getFdbyNickName(_extra_args[0]));
				}
				else
				{
					if (channel.isOp(_extra_args[0]) == false)
						{;} // ignored if is not op 
					channel.removeOp(_extra_args[0]);
				}
			}
		}
		if (_flag[i] == 'l')
		{
			// DEBUG // 
				std::cout << "rentre dans le l" << std::endl;
			//

			if (set_flag)
			{
				// DEBUG // 
				if (!_extra_args[0].empty())
					std::cout << _extra_args[0] << std::endl;
				else
					std::cout << "rien dans args[0]" << std::endl;
				//
				std::string::iterator it;
				for (it = _extra_args[0].begin(); it != _extra_args[0].end(); ++it)
				{
					if (std::isdigit(*it) == false)
					{
						_server.setBroadcast(ERR_UMODEUNKNOWNFLAG(_extra_args[0]), _user.getSocket());
						return;
					}
				}
				channel.setLimit(atoi(_extra_args[0].c_str()));
			}
			else
				channel.removeLimit();
		}
	}
	string msg = _user.getPrefix() + " " + _user.userMessageBuffer;
	_server.setBroadcast(msg, _user.getSocket());
	_server.setBroadcast(_channel, _user.getNickName(), msg);
}

