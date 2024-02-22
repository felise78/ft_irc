#include "User.hpp"
#include "../Channel/Channel.hpp"

#include <iostream> 

User::User(const std::string & nickname, const std::string & username) : _nickname(nickname), _username(username)
{
	std::cout << "User " << _username << " has been created" << std::endl;
}

User::~User()
{
	std::cout << "User " << _username << " has been destructed" << std::endl;
}

void	User::setSocket(const int & socket)
{
	_socket = socket;
}

void 	User::setNickname(const std::string & nickname)
{
	_nickname = nickname;
}

void 	User::setUsername(const std::string & username)
{
	_username = username;
}

void 	User::setPassword(const std::string & password)
{
	_password = password;
}

void	User::setChannel(const Channel & channel)
{
	_channels.insert(std::make_pair(channel.getName(), channel));

	/*Lorsque vous appelez insert, cela va ajouter un élément à la 
	map _channels où la clé sera le nom du canal et la valeur sera le canal 
	lui-même. Si la clé existe déjà, l'insertion échouera car les clés doivent
	être uniques dans une map. Si vous souhaitez remplacer le canal existant
	en cas de clé en double, vous pouvez utiliser operator[] à la place de 
	insert :   _channels[channel.getName()] = channel; */
}

const int & User::getSocket( void ) const
{
	return _socket;
}

const std::string& User::getNickname( void ) const
{
	return _nickname;
}

const std::string& User::getUsername( void ) const
{
	return _username;
}

const std::string& User::getPassword( void ) const
{
	return _password;
}