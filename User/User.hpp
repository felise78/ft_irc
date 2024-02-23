#ifndef USER_HPP
# define USER_HPP

	#include <string>
	#include <map>
	#include "../Channel/Channel.hpp"

	class User {

		private :
		int								_port;
		int								_socket;
		std::string						_hostName; // ..parsed in `UserRequestParsing` class..
		std::string 					_nickName;	// ..parsed in `UserRequestParsing` class..
		std::string 					_userName; // ..parsed in `UserRequestParsing` class..
		std::string						_realName; // ..parsed in `UserRequestParsing` class..
		std::string 					_password; // ..parsed in `UserRequestParsing` class..
		std::map<std::string, Channel*>	_channels;
		std::string						_clientMessageBuffer; // il y aura un type message ?
		std::string						_responseBuffer;
		bool							_authenticated;
		bool							_handshaked;
		// ..to use for composing the first response message to the client (RPL_WELCOME, RPL_YOURHOST, RPL_CREATED, RPL_MYINFO..)
		bool							_isOp; 
		bool							_canModifyTopic;
		// ce qui etait dans Operateur -> a mettre dans CommandHandler
		// void	_kick(User& user, Channel& channel);
		// void	_invite(User& user, Channel& channel);
		// void	_topic(Channel& channel);
		// void	_mode(const int& flag, Channel& channel, User& user);

		public :
		User(const int& fd);
		~User();
		// Setters //
		void	setPort(const int& port);
		void	setSocket(const int& socket);
		void 	setNickname(const std::string& nickname);
		void 	setUsername(const std::string& username);
		void 	setHostname(const std::string& hostname);
		void 	setPassword(const std::string& password);
		void	setChannel(Channel& channel);
		void	setIsOp( const bool& isOp );
		void	setCanModifyTopic( const bool& topic );
		// Getters //
		const int& getPort( void ) const;
		const int& getSocket( void ) const;
		const std::string& getNickname( void ) const;
		const std::string& getUsername( void ) const;
		const std::string& getHostname( void ) const;
		const std::string& getPassword( void ) const;
		Channel& getChannel( const std::string& name ) const;
		const std::map<std::string, Channel*>& getChannels( void ) const;
		const bool& getIsOp( void ) const;
		const bool& getCanModifyTopic( void ) const;

		void	printChannels( void ) const;   // for debug
		void	removeChannel(Channel& channel);
		
	};

#endif

// faire le getter et setter des requests buffers

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