#include "ModeHandler.hpp"

/* CONSTRUCTOR/DESTRUCTOR */
ModeHandler::ModeHandler(map<string, string>& commands, ServerManager& srv, User& user) : _commandsFromClient(commands), _server(srv), _user(user), n_flags(0), n_channels(0)
{
	if (_user.getStatus() != REGISTERED)
	{
		srv.setBroadcast(ERR_NOTREGISTERED(_server.hostname), _user.getFd());
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
	if (_commandsFromClient["params"].find("#") == std::string::npos) //removed & 
	{
		if (DEBUG)
			std::cout << "returns cuz no '#'" << std::endl;
		return 1;
	}
	stringstream params;
	params.str(_commandsFromClient["params"]);

	if (DEBUG)
		std::cout << "params : " << params.str() << std::endl;

	vector<string> args;
	string			tmp;
	while (getline(params, tmp, ' ')) //vector method needs to change. Should be closer to COMMAND/PARAMS type parsing. 
	{
		if (!tmp.empty())
			args.push_back(tmp);
	}

	if (DEBUG)
	{
		for (size_t i = 0; i < args.size(); i++)
			std::cout << "arg[" << i << "] : " << args[i] << " ";
		std::cout << std::endl;
		}
	
	for (size_t i = 0; i < args.size(); i++)
	{
		if (args[i][0] == '#')
		{
			n_channels++;
			if (_server.channelMap.find(args[i]) != _server.channelMap.end())
				_channel = args[i];
			else
			{
				_server.setBroadcast(ERR_NOSUCHCHANNEL(_server.hostname, _user.getNickName(), args[i]), _user.getFd());
				return 1;
			}
		}
		else if (args[i][0] == '+' || args[i][0] == '-')
		{
			// le comportement de irssi : il va concatener tout ce qui vient apres un + ou -
			// freenode ne return pas si il trouve un flag qu'il ne connait pas mais traite chaque flag.
			_flag = args[i];
			// for (size_t i = 1; i < _flag.size(); i++) //|| npos removed
			// {
			// 	const string modes = "itkol";
			// 	if (_flag.size() < 2)
			// 	{
			// 		_server.setBroadcast(ERR_UMODEUNKNOWNFLAG(_server.hostname, _user.getPrefix(), _flag[i]), _user.getFd());
			// 		return 1;
			// 	}
			// }
			n_flags++;
		}
		else
		{
			_extra_args.push_back(args[i]);
		}
	}

	if (DEBUG)
		std::cout << "nb chan : " << n_channels << std::endl;
	
	if (n_flags < 1)
	{
		_server.setBroadcast(ERR_NEEDMOREPARAMS(_server.hostname, _commandsFromClient["command"]), _user.getFd());
		return 1;
	}
	// if (n_flags < 1 || n_channels < 1)
	// {
	// 	_server.setBroadcast(ERR_NEEDMOREPARAMS(_server.hostname, _commandsFromClient["command"]), _user.getFd());
	// 	return 1;
	// }
	if (_extra_args.size() > 1 || n_flags > 1 || n_channels > 1)
	{
		_server.setBroadcast(ERR_TOOMANYTARGETS(_server.hostname, _commandsFromClient["command"]), _user.getFd());
		return 1;
	}
	Channel &c_tmp = _server.channelMap[_channel];
	std::string nickname = _user.getNickName();
	
	// checks if the user is op before executing mode 
	if (c_tmp.isOp(nickname) == true)
		return 0;
	else
	{
		if (DEBUG)
			std::cout << RED << "ici chanOp" << std::endl;
		_server.setBroadcast(ERR_CHANOPRIVSNEEDED(_server.hostname, _user.getNickName(), _channel), _user.getFd());
		return 1;
	}
}

void	ModeHandler::exec_mode()
{
	// format  : /MODE #channel flag [param]
	// ou
	// format  : /MODE nickname flag [param]

	bool	set_flag;
	std::map<std::string, Channel>::iterator it = _server.channelMap.find(_channel);
	if (it == _server.channelMap.end() || _flag.empty())
		return ;

	// DEBUG //////////////
	std::cout << RED;
	std::cout << "flag : " << _flag << std::endl;
	vector<string>::iterator it2 = _extra_args.begin();
	for( ; it2 != _extra_args.end(); ++it2)
		std::cout << "_extra_args : " << *it2 << " ";
	std::cout << RESET << std::endl;
	//////////////////////

	// settings
	Channel &channel = it->second;
	if (!(_flag.empty()) && _flag[0] == '+')
		set_flag = true;
	else if (!(_flag.empty()) && _flag[0] == '-')
		set_flag = false;
	// std::string chan_flags; //// RPL_flags // ???
	
	
	
	// main loop
	for (size_t i = 1; i < _flag.size(); i++)
	{
		if (_flag[i] == 'i')
		{
			if (DEBUG)
				std::cout << MAGENTA << "MODE 'i'" << RESET << std::endl;
			channel.setInvit(set_flag);
			// RPL_CHANNELMODEIS
		}
		else if (_flag[i] == 't')
		{
			if (DEBUG)
				std::cout << MAGENTA << "MODE 't'" << RESET << std::endl;
			channel.setTopicRestricted(set_flag);
		}
		/*
			 FLAG 'k' // CHANNEL PASSWORD MODE
		*/
		else if (_flag[i] == 'k')
		{
			if (DEBUG)
				std::cout << MAGENTA << "MODE 'k'" << RESET << std::endl;
			if (set_flag == false && channel.getProtected() == true)
			{
				channel.setProtected(false);
				channel.setKey("");
			}
			else if (channel.getProtected() == false && set_flag && !(_extra_args.empty())) 
			{
				channel.setKey(_extra_args[0]);
				//_server.setBroadcast(MODE_CHANNELMSGWITHPARAM(_user.getPrefix(), _channel, "k", _extra_args[0]), _user.getFd());
			}
			else if (_extra_args.empty())
			{
				_server.setBroadcast(ERR_EMPTYMODEPARAM(_server.hostname, _user.getNickName(), _channel, _flag[i]), _user.getFd());
				return;
			}
			
		}
		/*
			 FLAG 'o' // CHANOP MODE
		*/
		else if (_flag[i] == 'o')
		{
			if (DEBUG)
				std::cout << MAGENTA << "MODE 'o'" << RESET << std::endl;
			if (_extra_args.empty())
			{
				_server.setBroadcast(ERR_EMPTYMODEPARAM(_server.hostname, _user.getNickName(), _channel, _flag[i]), _user.getFd());
				return;
			}
			if (_server.usersMap.find(_server.getFdbyNickName(_extra_args[0])) == _server.usersMap.end())
			{
				_server.setBroadcast(ERR_NOSUCHNICK(_server.hostname, _user.getNickName(), _extra_args[0]), _user.getFd());
				return;
			}
			if (channel._users.find(_extra_args[0]) == channel._users.end())
			{
				_server.setBroadcast(ERR_NOTONCHANNEL(_server.hostname, _user.getNickName(), _channel), _user.getFd());
				return ;
			}
			if (set_flag)
			{
				if (channel.isOp(_extra_args[0]) == false)   // it should do nothing if the user is already op
				{
					channel.setOp(_extra_args[0]);
					_user.userMessageBuffer += MODE_USERMSG(_extra_args[0], "+o");
					//_server.setBroadcast(MODE_USERMSG(_extra_args[0], "+o"), _server.getFdbyNickName(_extra_args[0]));
				}
		}
			else
			{
				channel.removeOp(_extra_args[0]);
				_user.userMessageBuffer += MODE_USERMSG(_extra_args[0], "-o");
				//_server.setBroadcast(MODE_USERMSG(_extra_args[0], "-o"), _server.getFdbyNickName(_extra_args[0]));
			}
		}
		/*
			 FLAG 'l' // CHANNEL USER LIMIT 
		*/
		else if (_flag[i] == 'l')
		{
			if (DEBUG)
				std::cout << MAGENTA << "MODE 'l'" << RESET << std::endl;
			if (set_flag)
			{
				if (_extra_args.empty())
				{
					_server.setBroadcast(ERR_EMPTYMODEPARAM(_server.hostname, _user.getNickName(), _channel, _flag[i]), _user.getFd());
					return;
				}
				// protection if _extra_args contains letters (if so it could set the limit to 0)
				std::string::iterator it;
				for (it = _extra_args[0].begin(); it != _extra_args[0].end(); ++it)
				{
					if (std::isdigit(*it) == false)
					{
						_server.setBroadcast(ERR_INVALIDMODEPARAM(_server.hostname, _user.getNickName(), _channel, _flag[i], _extra_args[0]), _user.getFd());
						return;
					}
				}
				channel.setLimit(atoi(_extra_args[0].c_str())); // je pense il faut aussi envoyer un CHAN MODE MSG
			}
			else
				channel.removeLimit(); // je pense il faut aussi envoyer un CHAN MODE MSG
		}
		/*
			unknown flag
		*/
		else
		{
			_user.userMessageBuffer += ERR_UMODEUNKNOWNFLAG(_server.hostname, _user.getPrefix(), _flag[i]);
			//_server.setBroadcast(ERR_UMODEUNKNOWNFLAG(_server.hostname, _user.getPrefix(), _flag[i]), _user.getFd());
		}
	}
	/*
		sends reply 
	*/
	string msg = _user.getPrefix() + " " + _user.userMessageBuffer;
	if (DEBUG)
		std::cout << RED << "msg : " <<  msg << RESET << std::endl;
	_server.setBroadcast(msg, _user.getFd());
	_server.setBroadcast(_channel, _user.getNickName(), msg);
}

