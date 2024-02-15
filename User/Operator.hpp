#ifndef OPERATOR_HPP
# define OPERATOR_HPP

	#include "User.hpp"

	class Operator : public User 
	{
		private:
		void	kick(const User& user, const Channel& channel);
		void	invite(const User& user, const Channel& channel);
		void	topic(const Channel& channel);

		public : 
		Operator(std::string & nickname, std::string & username, std::string & password);
		~Operator();
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

#endif