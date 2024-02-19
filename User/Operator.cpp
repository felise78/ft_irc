#include "Operator.hpp"
#include "User.hpp"
#include "../Channel/Channel.hpp"
#include <iostream>

#define DEBUG 1

Operator::Operator(const std::string & nickname, const std::string & username) : User(nickname, username)
{
	_canModifyTopic = true;
}

Operator::~Operator()
{

}

const bool&	Operator::getTopic( void ) const
{
	return _canModifyTopic;
}

void	Operator::setTopic( const bool& topic )
{
	_canModifyTopic = topic;
}

void	Operator::_kick(User& user, Channel& channel)
{
	if (DEBUG)
	{
		user.getUsername();
		user.printChannels();
		channel.getName();
	}
	user.removeChannel(channel);
}

void	Operator::_invite(User& user, Channel& channel)
{
	// inviter un client // envoyer une request ?
}

void	Operator::_topic(Channel& channel, const Operator& op)
{
	std::cout << channel.getTheme() << std::endl;
	// modifier le theme ?
}

// pour la commande MODE il faudrait qu'au parsing je recoive juste la lettre qui suit le tiret

void	Operator::_mode(const int& flag, Channel& channel, Operator& op, User& user)
{
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
			// return if user is not op
			if (op.getTopic() == true)
				op.setTopic(false);
			else if (op.getTopic() == false)
				op.setTopic(true);
			break;
		}
		case 'k':
			break;
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