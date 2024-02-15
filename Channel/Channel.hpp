#ifndef CHANNEL_HPP
# define CHANNEL_HPP

	#include <map>
	#include <string>
	
	class User;

	class Channel {

		private :
			std::string						_name;
			std::string						_theme; // presentation du sujet du channel / regles
			std::string						_key; // channel password
			std::map<std::string, User> 	_users;
			int								_limit;

		public :
			Channel();
			~Channel();
			void	setName(const std::string & name);
			void	setUser(const User & user);
			const std::string & getName( void ) const;
			const User& getUser( const std::string & username );
	};

#endif