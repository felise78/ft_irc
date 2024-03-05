#ifndef SERVER_REPLIES_H
#define SERVER_REPLIES_H

/*From https://datatracker.ietf.org/doc/html/rfc2812#section-5*/

/* SUCCESSFUL REGISTRATION: */
#define RPL_WELCOME(nick, user, host) ":localhost 001 " + nick + " :Welcome to the Internet Relay Network " + nick "! " + user + "@" + host
#define RPL_YOURHOST(nick, servername, ver) ":localhost 002 " + nick + " :Your host is " + servername + ", running version " + ver
#define RPL_CREATED(nick, date) ":localhost 003 " + nick + " :This server was created " + date
#define RPL_MYINFO (nick) ":localhost 004 " + nick + " : ircserv.1 itkol"

/*The PING command is used to test the presence of an active client or server at the other 
end of the connection.  Servers send a PING message at regular intervals if no other activity 
detected coming from a connection.  If a connection fails to respond to a PING message within
a set amount of time, that connection is closed. A PING message MAY be sent even if the 
connection is active.*/
#define PING "PING :<servername>" //check

/*PRIVATE MESSAGE COMMAND	
PRIVMSG is used to send private messages between users, as well as to send messages to channels.
<msgtarget> is usually the nickname of the recipient of the message, or a channel name.*/
#define ERR_NORECIPIENT ":No recipient given (<command>)"
// code 411
#define ERR_NOTEXTTOSEND ":No text to send"					
// code 412
#define ERR_NOSUCHNICK "<nickname> :No such nick"
// code 401
#define ERR_TOOMANYTARGETS "<target> :<error code> recipients. <abort message>"	
// code 407
/*Sent to a user who is either (a) not on a channel which is mode +n 
or (b) not a chanop (or mode +v) on a channel which has mode +m set 
or where the user is banned and is trying to send a PRIVMSG message to that channel.*/
#define ERR_CANNOTSENDTOCHAN "<channel name> :Cannot send to channel"	// code 404
// code 404

/*KICK COMMAND*/
/*Returned to the client that it didn't supply enough parameters.*/
#define ERR_NEEDMOREPARAMS "<command> :Not enough parameters"
// code 461
/*the target user of the command is not on the given channel*/
#define ERR_USERNOTINCHANNEL "<nick> <channel> :They aren't on that channel" // code 441
// code 441
/*Used to indicate the given channel name is invalid.*/
#define ERR_NOSUCHCHANNEL "<channel name> :No such channel"
// code 403
/*Any command requiring 'chanop' privileges (such as MODE messages) MUST return this error 
if the client making the attempt is not a chanop on the specified channel.*/
#define ERR_CHANOPRIVSNEEDED "<channel> :You're not channel operator"
// code 482
/*Returned by the server whenever a client tries to perform a channel affecting command for which the
client isn't a member.*/
#define ERR_NOTONCHANNEL "<channel> :You're not on that channel"
// code 442

/*INVITE COMMAND*/
/*Returned when a client tries to invite a user to a channel they are already on.*/
#define ERR_USERONCHANNEL "<user> <channel> :is already on channel"

/*The attempted INVITE message was successful and is being passed onto the end client.*/
#define RPL_INVITING "<channel> <nick>"
// code 341
// ERR_NEEDMOREPARAMS (see KICK)
// ERR_NOTONCHANNEL (see KICK)
// ERR_CHANOPRIVSNEEDED (see KICK)
// ERR_NOSUCHNICK (see PRIVMSG)

/* TOPIC COMMAND: 
If the topic is set, RPL_TOPIC is sent back, else RPL_NOTOPIC.*/
#define RPL_TOPIC "<channel> :<topic>"
#define RPL_NOTOPIC "<channel> :No topic is set"
// code 331
#define ERR_NOCHANMODES "<channel> :Channel doesn't support modes"
// ERR_CHANOPRIVSNEEDED (see KICK)
// ERR_NOTONCHANNEL (see KICK)
// ERR_NEEDMOREPARAMS (see KICK)

/*MODE COMMAND*/
#define ERR_UNKNOWNMODE "<char> :is unknown mode char to me for <channel>"
// code 472
#define RPL_CHANNELMODEIS "<channel> <mode> <mode params>"

#define ERR_KEYSET "<channel> :Channel key already set"
// code 467
// ERR_USERNOTINCHANNEL (see KICK)
// ERR_NOCHANMODES (see TOPIC)
// ERR_NEEDMOREPARAMS (see KICK)
// ERR_CHANOPRIVSNEEDED (see KICK)


/* Client has successfully issued an OPER message and gained operator status.*/ 
//Don't know if required by subject? They specify chanop but not IRC op?
#define RPL_YOUREOPER ":You are now an IRC operator"

/*The command sent is unknown by the server.*/
#define ERR_UNKNOWNCOMMAND "<command> :Unknown command"
// code 421

/*Returned after receiving a NICK message which contains characters which do not 
fall in the defined set.  See section 2.3.1 for details on valid nicknames.*/
#define ERR_ERRONEUSNICKNAME "<nick> :Erroneous nickname"
// code 432

/*Nickname used*/
#define ERR_NICKNAMEINUSE "<nick> :Nickname is already in use"
// code 433







/*Returned to indicate a failed attempt at registering a connection for which a password 
was required and was either not given or incorrect.*/
#define ERR_PASSWDMISMATCH ":Password incorrect"

/*User doesn't have oper privileges*/
#define ERR_NOPRIVILEGES ":Permission Denied- You're not an IRC operator"



/*Any attempts to use the KILL command on a server are to be refused and this error returned 
directly to the client.*/
#define ERR_CANTKILLSERVER ":You can't kill a server!"

/*Returned by the server to indicate that a MODE message was sent with a nickname parameter 
and the mode flag sent was not recognized.*/
#define ERR_UMODEUNKNOWNFLAG ":Unknown MODE flag"

/*Error sent to any user trying to view or change the user mode for a user other than themselves.*/
#define ERR_USERSDONTMATCH ":Cannot change mode for other users"



#endif