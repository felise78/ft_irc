#ifndef CHANNEL_HPP
# define CHANNEL_HPP

	#include <map>
	#include <string>
	#include "../User/Operator.hpp"

	class User;
	class Operator;

	class Channel {

		private :
			std::string						_name;
			std::string						_theme; 	// theme and/or rules of channel
			std::string						_key; 		// channel password
			std::map<std::string, User*> 	_users;
			
			int								_limit; 	// max number of users
			bool							_invit; 	// channel requests invit or not

		public :
			Channel(const std::string& name);
			~Channel();
			// setters and getters
			void	setName(const std::string & name);
			void	setTheme(const std::string & theme);
			void	setKey(const std::string & key);
			void	setUser(User& user);
			void	setLimit(const int & limit);
			void	setInvit(const bool & invit);
			const std::string& 	getName( void ) const;
			const std::string& 	getTheme( void ) const;
			const std::string&	getKey( void ) const;
			User& getUser( const std::string & username ) const;
			const std::map<std::string, User*>& getUsers( void ) const;
			const int& getLimit( void ) const;
			const bool& getInvit( void ) const;

			void	removeUser(User& user);
			void	printUsers( void ) const; // for debug
	};

#endif