#ifndef OPERATOR_HPP
# define OPERATOR_HPP

	#include "User.hpp"

	class Operator : public User 
	{
		private:
		void	_kick(User& user, Channel& channel);
		void	_invite(User& user, Channel& channel);
		void	_topic(Channel& channel, const Operator& op);
		void	_mode(const int& flag, Channel& channel, Operator& op, User& user);
		bool	_canModifyTopic;

		public : 
		Operator(const std::string & nickname, const std::string & username);
		~Operator();
		const bool& getTopic( void ) const;
		void setTopic( const bool& topic );
	};

	
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


	// Les opérateurs du canal sont généralement désignés par un symbole "@" 
	// devant leur nom d'utilisateur dans la liste des utilisateurs du canal.

#endif