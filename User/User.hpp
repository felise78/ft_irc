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
		// ..to use for composing the first response message to the client (RPL_WELCOME, RPL_YOURHOST, RPL_CREATED, RPL_MYINFO..)
		bool							_isOp; 
		bool							_canModifyTopic;

		public :
		User() : _port(0), _socket(0), _hostName(""), _nickName(""), _userName(""), _password(""), _authenticated(false), _handshaked(false) {};
		~User() {};
		std::string						userMessageBuffer; // il y aura un type message ?
		std::string						responseBuffer;
		bool							_authenticated;
		bool							_handshaked;
		// Setters //
		void	setPort(const int& port);
		void	setSocket(const int& socket);
		void 	setNickName(const std::string& nickname);
		void 	setUserName(const std::string& username);
		void 	setHostName(const std::string& hostname);
		void 	setPassword(const std::string& password);
		void	setChannel(Channel& channel);
		void	setIsOp( const bool& isOp );
		void	setCanModifyTopic( const bool& topic );
		void			setAuthenticated(bool authenticated);
		void			setHandshaked(bool handshaked);
		// Getters //
		const int& getPort( void ) const;
		const int& getSocket( void ) const;
		const std::string& getNickName( void ) const;
		const std::string& getUserName( void ) const;
		const std::string& getHostName( void ) const;
		const std::string& getPassword( void ) const;
		Channel& getChannel( const std::string& name ) const;
		const std::map<std::string, Channel*>& getChannels( void ) const;
		const bool& getIsOp( void ) const;
		const bool& getCanModifyTopic( void ) const;
		bool			authenticated();
		bool			handshaked();

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