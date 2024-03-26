#include "Channel.hpp"

// -----------------------CONSTRUCTOR / DESTRUCTOR ---------------------------------//


Channel::Channel(const std::string& name) : _name(name), _topic(""), _key(""), _nb(0), _limit(0), 
_limited(false), _invit_only(false), _topic_restricted(false), _protected(false) {}

Channel::~Channel() {}

// ------------------------------ SETTERS ----------------------------------------- // 

void	Channel::setName(const std::string & name) { _name = name; }

void	Channel::setTopic(const std::string & topic) { _topic = topic; }

void	Channel::setKey(const std::string & key)
{ 
	
	_key = key;
	if (!key.empty())
		_protected = true;
	else 
		_protected = false;
}

void	Channel::setUser(User& user)
{
	_users[user.getNickName()] = &user;
	user.setChannel(*this);
	_nb++;
}

void	Channel::setOp(const std::string& nickname)
{
		_ops.push_back(nickname);
}

void	Channel::setNb(const int& nb) { _nb = nb; }

void	Channel::setLimit(const int & limit)
{
	_limit = limit;
	_limited = true;
}

void	Channel::setInvit(const bool & invit) { _invit_only = invit; }

void	Channel::setTopicRestricted(const bool& topic) { _topic_restricted = topic; }

void	Channel::setProtected(const bool& protecd) { _protected = protecd; }

void 	Channel::setInvited(const std::string& nickname)
{
	if (isInvited(nickname) == false)
		_invited.push_back(nickname);
}

void	Channel::setChannelModes(const char mode)
{
	_channelModes.push_back(mode);
}


// ---------------------------------- GETTERS -------------------------------------- // 


const std::string& Channel::getName( void ) const { return _name; }

const std::string& 	Channel::getTopic( void ) const { return _topic; }

const std::string&	Channel::getKey( void ) const { return _key; }

User& Channel::getUser( const std::string & nickname ) { return *_users.at(nickname); }

const int& Channel::getNb( void ) const { return _nb; }

const int& Channel::getLimit( void ) const { return _limit; }

const bool&	Channel::getLimited( void ) const { return _limited; }

const bool& Channel::getInvit( void ) const { return _invit_only; }

const bool& Channel::getTopicRestricted() const { return _topic_restricted; }

const bool& Channel::getProtected() const { return _protected; }

const std::string&	Channel::getChannelModes() const { return (_channelModes); }

// ------------------------------ MEMBER FUNCTIONS --------------------------------- // 

bool	Channel::isOp(const std::string& nickname)
{
	vector<string>::iterator it;
	vector<string>:: iterator last = _ops.end();

	for(it = _ops.begin(); it != last; ++it)
	{
		if (*it == nickname)
			return true;
	}
	return false;
}

bool	Channel::isInvited(const std::string& nickname)
{
	vector<string>::iterator it;
	vector<string>:: iterator last = _invited.end();

	for(it = _invited.begin(); it != last; ++it)
	{
		if (*it == nickname)
			return true;
	}
	return false;
}

void	Channel::removeUser(const std::string& nickname)
{
	std::map<std::string, User* >::iterator it;
	it = _users.find(nickname);
    if (it != _users.end())
	{
        _users.erase(it);
		_nb--;
	}
}

void	Channel::removeOp(const std::string& opNickname)
{
	for (std::vector<std::string>::iterator it = _ops.begin(); it != _ops.end(); ++it)
	{
        if (*it == opNickname)
		{
            _ops.erase(it);
            break; 
        }
    }
}

void	Channel::removeInvited(const std::string& nickname)
{
	for (std::vector<std::string>::iterator it = _invited.begin(); it != _invited.end(); ++it)
	{
        if (*it == nickname)
		{
            _invited.erase(it);
            break; 
        }
    }
}

void	Channel::removeTopic() { _topic = ""; }

void	Channel::removeLimit() { _limited = false; }

void	Channel::removeChannelMode(const char mode)
{
	size_t found = _channelModes.find(mode);
	if (found != std::string::npos)
		_channelModes.erase(found, 1);
}
