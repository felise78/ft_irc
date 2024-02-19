#ifndef USER_HPP
# define USER_HPP

	#include <string>
	#include <map>
	class Channel;

	class User {

		protected :
		int								_port;
		int								_socket;
		std::string 					_nickname;
		std::string 					_username;
		std::string						_hostname; 	// is it necessary if it is always localhost ?
		std::string 					_password;
		std::map<std::string, Channel*>	_channels;
		std::string						_clientMessageBuffer; // il y aura un type message ?
		std::string						_responseBuffer;
		bool							_isOp; 		// est un operateur ou pas

		public :
		User(const std::string & nickname, const std::string & username);
		virtual ~User();
		void	setPort(const int& port);
		void	setSocket(const int& socket);
		void 	setNickname(const std::string& nickname);
		void 	setUsername(const std::string& username);
		void 	setHostname(const std::string& hostname);
		void 	setPassword(const std::string& password);
		void	setChannel(Channel& channel);
		void	setIsOp( const bool& isOp );
		const int& getPort( void ) const;
		const int& getSocket( void ) const;
		const std::string& getNickname( void ) const;
		const std::string& getUsername( void ) const;
		const std::string& getHostname( void ) const;
		const std::string& getPassword( void ) const;
		Channel& getChannel( const std::string& name ) const;
		const std::map<std::string, Channel*>& getChannels( void ) const;
		const bool& isOp( void ) const;

		void	removeChannel(Channel& channel);
		void	printChannels( void ) const;
	};

#endif

// faire le getter et setter des requests buffers