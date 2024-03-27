#include "ModeHandler.hpp"

/* CONSTRUCTOR/DESTRUCTOR */
ModeHandler::ModeHandler(map<string, string>& commands, ServerManager& srv, User& user) : _commandsFromClient(commands), _server(srv), _user(user), n_flags(0), n_channels(0), _success(false)
{
	if (_user.getStatus() != REGISTERED)
	{
		srv.setBroadcast(ERR_NOTREGISTERED(_server.hostname), _user.getFd());
		return ;
	}
	_user.userMessageBuffer.clear();
	if (parse_errors() != 0)
		return ;
	exec_mode();
}

ModeHandler::~ModeHandler()
{
}

/* PARSING */
int	ModeHandler::parse_errors()
{
	if (_commandsFromClient["params"].find("#") == std::string::npos)
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
		if (args[i][0] == '#' && n_channels == 0)
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
			_flag.push_back(args[i]);
			n_flags++;
		}
		else
		{
			_extra_args.push_back(args[i]);
		}
	}
	if (n_flags < 1 || n_channels < 1)
	{
		_server.setBroadcast(ERR_NEEDMOREPARAMS(_server.hostname, _commandsFromClient["command"]), _user.getFd());
		return 1;
	}
	if (n_channels > 1)
	{
		_server.setBroadcast(ERR_TOOMANYTARGETS(_server.hostname, _commandsFromClient["command"]), _user.getFd());
		return 1;
	}
	Channel &c_tmp = _server.channelMap[_channel];
	std::string nickname = _user.getNickName();
	if (c_tmp.isOp(nickname) == true)
		return 0;
	else
	{ 
		_server.setBroadcast(ERR_CHANOPRIVSNEEDED(_server.hostname, _user.getNickName(), _channel), _user.getFd());
		return 1;
	}
}

/* EXECUTION */
void	ModeHandler::exec_mode()
{
	string add;
	string remove;
	std::map<std::string, Channel>::iterator it = _server.channelMap.find(_channel);
	if (it == _server.channelMap.end() || _flag.empty())
		return ;
	// Handles all flags
	Channel &channel = it->second;
	for (size_t a = 0; a < _flag.size(); a++)
		handle_flag(_flag[a], channel, add, remove);
	if (!_extra_args.empty())
	{
		for (size_t i = 0; i < _extra_args.size(); i++)
		{
			if (_extra_args[i][0] != '+' && _extra_args[i][0] != '-')
				_extra_args[i] = "+" + _extra_args[i];
			handle_flag(_extra_args[i], channel, add, remove);
		}
	}
	//Formulate reply
	const string prefix = _user.getPrefix();
	string params;
	if (!add.empty())
	{
		for (size_t i = 1; i < add.size(); i++)
		{
			size_t found = channel.getChannelModes().find(add[i]);
			if (found != string::npos)
				add.erase(add.find(add[i]));
			else
				channel.setChannelModes(add[i]);
		}
		if (add.size() > 1)
			params += add;
	}
	if (!remove.empty())
	{
		if (add.size() > 1)
			params += " ";
		for (size_t i = 1; i < remove.size(); i++)
		{
			if (channel.getChannelModes().find(remove[i]) != string::npos)
				channel.removeChannelMode(remove[i]);
			else
				remove.erase(remove.find(remove[i]));
		}
		if (remove.size() > 1)
			params += remove;
	}
	if (!_argsEnd.empty())
	{
		for (size_t i = 0; i < _argsEnd.size(); i++)
		{
			params += " ";
			params += _argsEnd[i];
		}

	}
	if (_success)
		_user.userMessageBuffer += RPL_CHANNELMODEIS(prefix, _user.getNickName(), _channel, params);
	string msg = _user.userMessageBuffer;
	_server.setBroadcast(msg, _user.getFd());
	_server.setBroadcast(_channel, _user.getNickName(), msg);
}

void	ModeHandler::handle_flag(string const& flag, Channel &channel, string& add, string& remove)
{
	bool	set_flag;
	if (!(flag.empty()) && flag[0] == '+' && add.empty())
	{
		set_flag = true;
		add += "+";
	}
	else if (!(flag.empty()) && flag[0] == '-' && remove.empty())
	{
		set_flag = false;
		remove += "-";
	}	
	for (size_t i = 1; i < flag.size(); i++)
	{
		if (flag[i] == 'i') // CHANNEL INVITE MODE
			handle_i(set_flag, channel, add, remove);
		else if (flag[i] == 't') // CHANNEL TOPIC MODE
			handle_t(set_flag, channel, add, remove);
		else if (flag[i] == 'k') // CHANNEL PASSWORD MODE
			handle_k(set_flag, channel, flag, add, remove);			
		else if (flag[i] == 'o') // CHANOP MODE
			handle_o(flag, set_flag, channel, add, remove);
		else if (flag[i] == 'l') // CHANNEL USER LIMIT 
			handle_l(set_flag, flag, channel, add, remove);
		else // unknown flag 
			_user.userMessageBuffer += ERR_UMODEUNKNOWNFLAG(_server.hostname, _user.getPrefix(), flag[i]);
	}
}

void	ModeHandler::handle_i(bool set_flag, Channel &channel, string& add, string& remove)
{
	if (set_flag == channel.getInvit())
		return ; 
	channel.setInvit(set_flag);
	_success = true;
	if (set_flag && add.find('i') == string::npos)
		add += "i";
	else if (set_flag == false && remove.find('i') == string::npos)
		remove += "i";
}

void	ModeHandler::handle_t(bool set_flag, Channel &channel, string& add, string& remove)
{
	if (set_flag == channel.getTopicRestricted())
		return ; 
	channel.setTopicRestricted(set_flag);
	_success = true;
	if (set_flag && add.find('t') == string::npos)
		add += "t";
	else if (set_flag == false && remove.find('t') == string::npos)
		remove += "t";
}

void	ModeHandler::handle_k(bool set_flag, Channel &channel, string const& flag, string& add, string& remove)
{
	// Remove a password that is set
	if (set_flag == false && channel.getProtected() == true)
	{
		_argsEnd.push_back(channel.getKey());
		channel.setProtected(false);
		channel.setKey("");
		if (remove.find('k') == string::npos)
			remove += "k";
		_success = true;
	}
	// Add a password to unprotected channel
	else if (channel.getProtected() == false && set_flag && !(_extra_args.empty())) 
	{
		channel.setKey(_extra_args[0]);
		if (add.find('k') == string::npos)	
			add += "k";
		_argsEnd.push_back(_extra_args[0]);
		_extra_args.erase(_extra_args.begin());
		_success = true;
	}
	// If request to set password where one already exists
	else if (channel.getProtected() == true && set_flag && !(_extra_args.empty()))
	{
		_server.setBroadcast(ERR_KEYSET(_server.hostname, _channel), _user.getFd());
		return;
	}
	// If password param is missing
	else if (_extra_args.empty())
	{
		_server.setBroadcast(ERR_EMPTYMODEPARAM(_server.hostname, _user.getNickName(), _channel, flag), _user.getFd());
		return;
	}
}

void	ModeHandler::handle_o(string const& flag, bool set_flag, Channel& channel, string& add, string& remove)
{
	// Operator name missing
	if (_extra_args.empty())
	{
		_server.setBroadcast(ERR_EMPTYMODEPARAM(_server.hostname, _user.getNickName(), _channel, flag), _user.getFd());
		return;
	}
	// User specified doesn't exist
	if (_server.usersMap.find(_server.getFdbyNickName(_extra_args[0])) == _server.usersMap.end())
	{
		_server.setBroadcast(ERR_NOSUCHNICK(_server.hostname, _user.getNickName(), _extra_args[0]), _user.getFd());
		_extra_args.erase(_extra_args.begin());
		return;
	}
	// User specified is not channel member
	if (channel._users.find(_extra_args[0]) == channel._users.end())
	{
		_server.setBroadcast(ERR_NOTONCHANNEL(_server.hostname, _user.getNickName(), _channel), _user.getFd());
		_extra_args.erase(_extra_args.begin());
		return ;
	}
	// Set user as operator
	if (set_flag && channel.isOp(_extra_args[0]) == false)
	{
		channel.setOp(_extra_args[0]);
		_user.userMessageBuffer += MODE_USERMSG(_extra_args[0], "+o");
		_argsEnd.push_back(_extra_args[0]);
		_extra_args.erase(_extra_args.begin());
		if (add.find('o') == string::npos)
			add += "o";
		_success = true;
	}
	// Remove user as operator 
	else if (set_flag == false && channel.isOp(_extra_args[0]) == true)
	{
		channel.removeOp(_extra_args[0]);
		_user.userMessageBuffer += MODE_USERMSG(_extra_args[0], "-o");
		_argsEnd.push_back(_extra_args[0]);
		_extra_args.erase(_extra_args.begin());
		if (remove.find('o') == string::npos)
			remove += "o";
		_success = true;
	}
}

void	ModeHandler::handle_l(bool set_flag, string const& flag, Channel& channel, string& add, string& remove)
{
	//Set limit
	if (set_flag)
	{
		//Limit not specified
		if (_extra_args.empty())
		{
			_server.setBroadcast(ERR_EMPTYMODEPARAM(_server.hostname, _user.getNickName(), _channel, flag), _user.getFd());
			return;
		}
		//Limit invalid
		std::string::iterator it;
		for (it = _extra_args[0].begin(); it != _extra_args[0].end(); ++it)
		{
			if (std::isdigit(*it) == false)
			{
				_server.setBroadcast(ERR_INVALIDMODEPARAM(_server.hostname, _user.getNickName(), _channel, flag, _extra_args[0]), _user.getFd());
				_extra_args.erase(_extra_args.begin());
				return;
			}
		}
		channel.setLimit(atoi(_extra_args[0].c_str()));
		_argsEnd.push_back(_extra_args[0]);
		_extra_args.erase(_extra_args.begin());
		if (add.find('l') == string::npos)	
			add += "l";
		_success = true;
	}
	//Remove limit
	else if (set_flag == false && channel.getChannelModes().find('l') != string::npos)
	{
		channel.removeLimit();
		if (remove.find('l') == string::npos)
			remove += "l";
		_success = true;
	}
}
