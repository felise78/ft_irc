#ifndef USER_HPP
# define USER_HPP

	#include <string>
	#include <map>
	class Channel;

	class User {

		protected :

		int								_socket;
		std::string 					_nickname;
		std::string 					_username;
		std::string 					_password;
		std::map<std::string, Channel*>	_channels;

		public :
		User(const std::string & nickname, const std::string & username);
		virtual ~User();
		void	setSocket(const int & socket);
		void 	setNickname(const std::string & nickname);
		void 	setUsername(const std::string & username);
		void 	setPassword(const std::string & password);
		void	setChannel(Channel& channel);
		const int & getSocket( void ) const;
		const std::string& getNickname( void ) const;
		const std::string& getUsername( void ) const;
		const std::string& getPassword( void ) const;
		Channel& getChannel( const std::string& name ) const;
		const std::map<std::string, Channel*>& getChannels( void ) const; 
	};

#endif