#ifndef USER_HPP
# define USER_HPP

	#include <string>
	#include <map>
	#include "../Channel/Channel.hpp"

	class User {

		protected :

		int								_socket;
		std::string 					_nickname;
		std::string 					_username;
		std::string 					_password;
		std::map<std::string, Channel>	_channels;

		public :

		User(const std::string & nickname, const std::string & username, const std::string & password);
		~User();
		void	setSocket(const int & socket);
		void 	setNickname(const std::string & nickname);
		void 	setUsername(const std::string & username);
		void 	setPassword(const std::string & password);
		void	setChannel(const Channel & channel);
		const int & getSocket( void ) const;
		const std::string& getNickname( void ) const;
		const std::string& getUsername( void ) const;
		const std::string& getPassword( void ) const;
		const Channel& getChannel( void ) const; // je suis pas sure pour celui la ? 
		// j'envoie toute la liste ? 
	};

#endif