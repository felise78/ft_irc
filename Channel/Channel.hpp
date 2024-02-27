#ifndef CHANNEL_HPP
# define CHANNEL_HPP

	#include <map>
	#include <string>
	#include <vector>

	class User;
	class Operator;

	class Channel {

		private :
			std::string						_name;
			std::string						_theme; 	// theme and/or rules of channel
			std::string						_key; 		// channel password
			std::map<std::string, User*> 	_users;
			std::vector<std::string>		_ops;
			int								_nb;
			int								_limit; 	// max number of users
			bool							_limited; 
			bool							_invit_only;
			bool							_topic_restricted;	

		public :
			Channel(const std::string& name);
			~Channel();
			// setters 
			void	setName(const std::string & name);
			void	setTheme(const std::string & theme);
			void	setKey(const std::string & key);
			void	setUser(User& user);
			void	setOp(const std::string& nickname);
			void	setNb(const int& nb);
			void	setLimit(const int & limit);
			void	setInvit(const bool & invit);
			void	setTopicRestricted(const bool& topic);
			// getters
			const std::string& 	getName( void ) const;
			const std::string& 	getTheme( void ) const;
			const std::string&	getKey( void ) const;
			User& getUser( const std::string & nickname ) const;
			const std::string& getOp( const std::string & nickname ) const;
			const int& getNb( void ) const;
			const int& getLimit( void ) const;
			const bool& getInvit( void ) const;
			const bool& getTopicRestricted() const;

			void	removeUser(User& user);
			void	removeOp(const std::string& opNickname);
			void	printUsers( void ) const; // for debug
	};

#endif