#ifndef SERVER_REPLIES_H
#define SERVER_REPLIES_H

/*From https://datatracker.ietf.org/doc/html/rfc2812#section-5*/

/* SUCCESSFUL REGISTRATION: */
#define RPL_WELCOME "Welcome to the Internet Relay Network <nick>!<user>@<host>"
#define RPL_YOURHOST "Your host is <servername>, running version <ver>"
#define RPL_CREATED "This server was created <date>"
#define RPL_MYINFO "<servername> <version> <available user modes><available channel modes>"

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
#define ERR_NOTEXTTOSEND ":No text to send"
#define ERR_NOSUCHNICK "<nickname> :No such nick/channel"
#define ERR_TOOMANYTARGETS "<target> :<error code> recipients. <abort message>"
/*Sent to a user who is either (a) not on a channel which is mode +n 
or (b) not a chanop (or mode +v) on a channel which has mode +m set 
or where the user is banned and is trying to send a PRIVMSG message to that channel.*/
#define ERR_CANNOTSENDTOCHAN "<channel name> :Cannot send to channel"

/*KICK COMMAND*/
/*Returned to the client that it didn't supply enough parameters.*/
#define ERR_NEEDMOREPARAMS "<command> :Not enough parameters"
/*the target user of the command is not on the given channel*/
#define ERR_USERNOTINCHANNEL "<nick> <channel> :They aren't on that channel"
/*Used to indicate the given channel name is invalid.*/
#define ERR_NOSUCHCHANNEL "<channel name> :No such channel"
/*Any command requiring 'chanop' privileges (such as MODE messages) MUST return this error 
if the client making the attempt is not a chanop on the specified channel.*/
#define ERR_CHANOPRIVSNEEDED "<channel> :You're not channel operator"
/*Returned by the server whenever a client tries to perform a channel affecting command for which the
client isn't a member.*/
#define ERR_NOTONCHANNEL "<channel> :You're not on that channel"

/*INVITE COMMAND*/
/*Returned when a client tries to invite a user to a channel they are already on.*/
#define ERR_USERONCHANNEL "<user> <channel> :is already on channel"
/*The attempted INVITE message was successful and is being passed onto the end client.*/
#define RPL_INVITING "<channel> <nick>"
// ERR_NEEDMOREPARAMS (see KICK)
// ERR_NOTONCHANNEL (see KICK)
// ERR_CHANOPRIVSNEEDED (see KICK)
// ERR_NOSUCHNICK (see PRIVMSG)

/* TOPIC COMMAND: 
If the topic is set, RPL_TOPIC is sent back, else RPL_NOTOPIC.*/
#define RPL_TOPIC "<channel> :<topic>"
#define RPL_NOTOPIC "<channel> :No topic is set"
#define ERR_NOCHANMODES "<channel> :Channel doesn't support modes"
// ERR_CHANOPRIVSNEEDED (see KICK)
// ERR_NOTONCHANNEL (see KICK)
// ERR_NEEDMOREPARAMS (see KICK)

/*MODE COMMAND*/
#define ERR_UNKNOWNMODE "<char> :is unknown mode char to me for <channel>"
#define RPL_CHANNELMODEIS "<channel> <mode> <mode params>"
#define ERR_KEYSET "<channel> :Channel key already set"
// ERR_USERNOTINCHANNEL (see KICK)
// ERR_NOCHANMODES (see TOPIC)
// ERR_NEEDMOREPARAMS (see KICK)
// ERR_CHANOPRIVSNEEDED (see KICK)


/* Client has successfully issued an OPER message and gained operator status.*/ 
//Don't know if required by subject? They specify chanop but not IRC op?
#define RPL_YOUREOPER ":You are now an IRC operator"

/*The command sent is unknown by the server.*/
#define ERR_UNKNOWNCOMMAND "<command> :Unknown command"

/*Returned after receiving a NICK message which contains characters which do not 
fall in the defined set.  See section 2.3.1 for details on valid nicknames.*/
#define ERR_ERRONEUSNICKNAME "<nick> :Erroneous nickname"

/*Nickname used*/
#define ERR_NICKNAMEINUSE "<nick> :Nickname is already in use"







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