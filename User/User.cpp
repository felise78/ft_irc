#include "User.hpp"
#include "../Channel/Channel.hpp"
#include <iostream> 

User::User(const int& fd) : _socket(fd)
{
	//std::cout << "User has been created" << std::endl;
}

User::~User()
{
	//std::cout << "User " << _userName << " has been destructed" << std::endl;
}

// --------------------------------------- SETTERS ---------------------------------------- // 

void	User::setPort(const int& port)
{
	_port = port;
}

void	User::setSocket(const int& socket)
{
	_socket = socket;
}

void 	User::setNickname(const std::string& nickname)
{
	_nickName = nickname;
}

void 	User::setUsername(const std::string& username)
{
	_userName = username;
}

void 	User::setHostname(const std::string& hostname)
{
	_hostName = hostname;
}

void 	User::setPassword(const std::string& password)
{
	_password = password;
}

void	User::setChannel(Channel& channel)
{
	//_channels.insert(std::make_pair(channel.getName(), channel));
	_channels[channel.getName()] = &channel;
	// checker si le channel existe deja et voir quel comportement je veux
}

void	User::setIsOp( const bool& isOp )
{
	_isOp = isOp;
}

void	User::setCanModifyTopic( const bool& topic )
{
	_canModifyTopic = topic;
}

// ---------------------------------------- GETTERS ----------------------------------------- // 

const int& User::getPort( void ) const
{
	return _port;
}

const int& User::getSocket( void ) const
{
	return _socket;
}

const std::string& User::getNickname( void ) const
{
	return _nickName;
}

const std::string& User::getUsername( void ) const
{
	return _userName;
}

const std::string& User::getHostname( void ) const
{
	return _hostName;
}

const std::string& User::getPassword( void ) const
{
	return _password;
}

Channel& User::getChannel( const std::string& name ) const
{
	return *_channels.at(name);
}

const std::map<std::string, Channel*>& User::getChannels( void ) const
{
	return _channels;
} 

const bool& User::getIsOp( void ) const
{
	return _isOp;
}

const bool&	User::getCanModifyTopic( void ) const
{
	return _canModifyTopic;
}

// ----------------------------------- MEMBER FUNCTIONS ------------------------------------- // 

void	User::removeChannel(Channel& channel)
{
	std::map<std::string, Channel*>::iterator it;
	it = _channels.find(channel.getName());
    if (it != _channels.end())
	{
		channel.removeUser(*this);
        _channels.erase(it);
	}
}

void User::printChannels( void ) const
{
	std::map<std::string, Channel*>::const_iterator it;

    std::cout << "Channels in this user:" << std::endl;
    for ( it = _channels.begin(); it != _channels.end(); ++it)
        std::cout << it->second->getName() << std::endl;
}

void	User::_kick(User& user, Channel& channel)
{
	//if (DEBUG)
	{
		user.getUsername();
		user.printChannels();
		channel.getName();
	}
	user.removeChannel(channel);
}

void	User::_invite(User& user, Channel& channel)
{
	// inviter un client // envoyer une request ?
	(void)user;
	(void)channel;
}

void	User::_topic(Channel& channel)
{
	if (_isOp == false)
		return;
	std::cout << channel.getTheme() << std::endl;
	// modifier le theme ?
	// if (_canModifyTopic)
}

void	User::_mode(const int& flag, Channel& channel, User& user)
{
	if (_isOp == false)
		return;

	switch (flag)
	{
		case 'i':
		{
			if (channel.getInvit() == true)
				channel.setInvit(false);
			else if (channel.getInvit() == false)
				channel.setInvit(true);
			break;
		}
		case 't':
		{
			if (user.getCanModifyTopic() == true)
				user.setCanModifyTopic(false);
			else if (user.getCanModifyTopic() == false)
				user.setCanModifyTopic(true);
			break;
		}
		case 'k':
		{
			break;
		}
		case 'o':
		{

		}
		case 'l':
			break;
		default:
			; // error wrong flag
	}
}

	// COMMANDES SPECIFIQUES AUX OPERATEURS : 

	// KICK - Ejecter un client du channel
	// INVITE - Inviter un client au channel
	// TOPIC - Modifier ou afficher le thème du channel
	// MODE - Changer le mode du channel :
	// 	— i : Définir/supprimer le canal sur invitation uniquement
	// 	— t : Définir/supprimer les restrictions de la commande TOPIC pour les opé-
	// 		rateurs de canaux
	// 	— k : Définir/supprimer la clé du canal (mot de passe)
	// 	— o : Donner/retirer le privilège de l’opérateur de canal
	// 	— l : Définir/supprimer la limite d’utilisateurs pour le canal