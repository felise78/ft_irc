#ifndef CHANNEL_HPP
# define CHANNEL_HPP

	#include <iostream>
	#include <map>
	#include <string>
	#include <vector>

	class User;
	class Operator;
	
	#include "../Server/ServerManager.hpp"
	#include "../User/User.hpp"

	class ServerManager;

	using namespace std;

	class Channel {

		private :
			std::string						_name;
			std::string						_topic; 
			std::string						_key; 
			int								_nb;
			int								_limit; 
			bool							_limited; 
			bool							_invit_only;
			bool							_topic_restricted;
			bool							_protected; 

		public :
			Channel() {}
			Channel(const std::string& name);
			~Channel();

			std::map<std::string, User *> 	_users;
			std::vector<std::string>		_ops;
			std::vector<std::string>		_invited;
			// setters 
			void	setName(const std::string & name);
			void	setTopic(const std::string & topic);
			void	setKey(const std::string & key);
			void	setUser(User& user);
			void	setOp(const std::string& nickname);
			void	setNb(const int& nb);
			void	setLimit(const int & limit);
			void	setInvit(const bool & invit);
			void	setTopicRestricted(const bool& topic);
			void	setProtected(const bool& protecd);
			void 	setInvited(const std::string& nickname);
			// getters
			const std::string& 	getName( void ) const;
			const std::string& 	getTopic( void ) const;
			const std::string&	getKey( void ) const;
			User& getUser( const std::string & nickname );
			const std::string& getOp( const std::string & nickname ) const;
			const int& getNb( void ) const;
			const int& getLimit( void ) const;
			const bool& getLimited( void ) const;
			const bool& getInvit( void ) const;
			const bool& getTopicRestricted() const;
			const bool& getProtected() const;

			bool	isOp(const std::string& nickname);
			bool	isInvited(const std::string& nickname);
			void	removeUser(const std::string& nickname);
			void	removeOp(const std::string& opNickname);
			void 	removeInvited(const std::string& nickname);
			void	removeTopic();
			void	removeLimit();
	};

#endif