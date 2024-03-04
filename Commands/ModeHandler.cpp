#include "ModeHandler.hpp"

/* CONSTRUCTOR/DESTRUCTOR */
ModeHandler::ModeHandler(map<string, string>& commands, ServerManager& srv) : _commandsFromClient(commands), _server(srv), n_flags(0), n_channels(0), _channel(NULL), _i(false), _t(false), _k(false), _o(false), _l(false)
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
	stringstream params = commandsFromClient["params"];
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
	for (; it != args.end(); i++)
	{
		if ((*it)[0] == '#' || (*it)[0] == '&')
		{
			n_channels++;
			channelIt	it2 = server.channelMap.find(*it);
			if (it2 != server.channelMap.end())
				_channel = it2;
			else
			{
				// send_error_msg to client, ERR_NOSUCHCHANNEL
				return 1;
			}
		}
		if ((*it)[0] == '+' || (*it)[0] == '-')
		{
			
			// check if flag valid else ERR_UMODEUNKNOWNFLAG, return 1
			n_flags++;
		}
	}
	if (n_flags != 1 || n_channels != 1) {}// - wrong no of flags or channels 461 ERR_NEEDMOREPARAMS, return 1

	// if (requesting user is not op of channel, return ERR_CHANOPRIVSNEEDED, return 1)
	if (user.getNickName())

	return (0);
}

void	ModeHandler::exec_mode()
{

}

