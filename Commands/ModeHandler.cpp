#include "ModeHandler.hpp"

/* CONSTRUCTOR/DESTRUCTOR */
ModeHandler::ModeHandler(map<string, string>& commands, ServerManager& srv, User& user) : _commandsFromClient(commands), _server(srv), _user(user), n_flags(0), n_channels(0), _channel(NULL)
{
	if (DEBUG)
		cout << "ModeHandler constructor called" << endl;

	if (parse_errors()) //within parse_errors, need to call a method in ServerManager to send_error_msg to client. 
		return ;
	exec_mode();	
}

ModeHandler::~ModeHandler()
{
	if (DEBUG)
		cout << "ModeHandler destructor called" << endl;
}

int	ModeHandler::parse_errors()
{
	stringstream params;
	params.str(_commandsFromClient["params"]);
	vector<string> args;
	string			tmp;
	while (getline(params, tmp, ' '))
	{
		if (!tmp.empty())
			args.push_back(tmp);
	}
	typedef vector<string>::iterator stringVecIt;
	typedef map<string, Channel*>::iterator channelIt;

	stringVecIt it = args.begin();
	for (; it != args.end(); it++)
	{
		if ((*it)[0] == '#' || (*it)[0] == '&')
		{
			n_channels++;
			channelIt	it2 = (_server.channelMap).find(*it);
			if (it2 != (_server.channelMap).end())
				_channel = it2->second;
			else
			{
				// send_error_msg to client, ERR_NOSUCHCHANNEL
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
					// ERR_UMODEUNKNOWNFLAG;
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
	if (n_flags != 1 || n_channels != 1)
	{
		// 461 ERR_NEEDMOREPARAMS,
		return 1;
	}
	if (_extra_args.size() > 1)
	{
		// 461 ERR_NEEDMOREPARAMS,
		return 1;
	}
	if (_channel->isOp(_user.getNickName()))
		return 0;
	else
	{
		// message ERR_CHANOPRIVSNEEDED
		return 1;
	}
}

void	ModeHandler::exec_mode()
{
	bool	set_flag;

	if (!(_flag.empty()) && _flag[0] == '+')
		set_flag = true;
	if (!(_flag.empty()) && _flag[0] == '-')
		set_flag = false;
	for (size_t i = 1; i < _flag.size(); i++)
	{
		if (_flag[i] == 'i')
			_channel->setInvit(set_flag);
		if (_flag[i] == 't')
			_channel->setTopicRestricted(set_flag);
		if (_flag[i] == 'k')
		{
			_channel->setProtected(set_flag);
			if (!_extra_args.empty())
				_channel->setKey(_extra_args[0]);
		}
		if (_flag[i] == 'o')
		{
			if (set_flag)
				_channel->setOp(_extra_args[0]);
			else
				_channel->removeOp(_extra_args[0]);
		}
		if (_flag[i] == 'l')
		{
			if (set_flag)
				_channel->setLimit(atoi(_extra_args[0].c_str()));
			else
				_channel->removeLimit();
		}
	}
}

