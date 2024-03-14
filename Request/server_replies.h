#ifndef SERVER_REPLIES_H
#define SERVER_REPLIES_H

#define RPL_WELCOME(nick, host) (":localhost 001 " + nick + " :Welcome to the Internet Relay Network " + nick + "!" + nick + "@" + host + "\r\n")
#define RPL_YOURHOST(nick) (":localhost 002 " + nick + " :Your host is ircserv, running version ircserv.1\r\n")
#define RPL_CREATED(nick, date) (":localhost 003 " + nick + " :This server was created " + date + "\r\n")
#define RPL_MYINFO(nick) (":localhost 004 " + nick + " : ircserv.1 modes itkol\r\n")

/*PASS / USER*/
#define ERR_NOTREGISTERED ":localhost 451 :You have not registered\r\n"
#define ERR_ALREADYREGISTRED ":localhost 462 ::Unauthorized command (already registered)\r\n"
#define ERR_PASSWDMISMATCH ":localhost 464 :Password incorrect\r\n"
#define PING(servername) ("PING :" + servername + "\r\n")
#define ERR_UNKNOWNCOMMAND(command) (":localhost 421 " + command " :Unknown command\r\n")

/*PRIVMSG*/
#define ERR_NOSUCHNICK(nickname) (":localhost 401 " + nickname + " :No such nick\r\n")
#define ERR_CANNOTSENDTOCHAN(channel) (":localhost 404 " channel + " :Cannot send to channel\r\n")
#define ERR_TOOMANYTARGETS(target) (":localhost 407 " + target + ":407 recipients. Use one target only.\r\n")
#define ERR_NORECIPIENT(command) (":localhost 411 :No recipient given " + command + "\r\n")
#define ERR_NOTEXTTOSEND ":localhost 412 :No text to send\r\n"
# define RPL_PRIVMSG(nick, target, message) (nick + " PRIVMSG " + target + " " + message + "\r\n") // nick is with prefix


/*KICK COMMAND*/
#define ERR_NOSUCHCHANNEL(channel) (":localhost 403 " + channel + " :No such channel\r\n")
#define ERR_USERNOTINCHANNEL(nick, channel) (":localhost 441 " + nick + " " + channel + " :They aren't on that channel\r\n")
#define ERR_NOTONCHANNEL(channel) (":localhost 442 " + channel + " :You're not on that channel\r\n")
#define ERR_NEEDMOREPARAMS(command) (":localhost 461 " + command + " :Not enough parameters\r\n")
#define ERR_CHANOPRIVSNEEDED(channel) (":localhost 482 " + channel + " :You're not channel operator\r\n")
#define RPL_KICK(user_id, channel, kicked, reason) (user_id + " KICK " + channel + " " + kicked + " " + reason + "\r\n")

/*INVITE COMMAND*/
#define RPL_INVITING(channel, nick) (":localhost 341 " + channel + " " + nick + "\r\n")
#define ERR_USERONCHANNEL(user, channel) (":localhost 442 " + user + " " + channel + " :is already on channel\r\n")

/* TOPIC COMMAND: */
#define RPL_NOTOPIC(channel) (":localhost 331 " + channel + " :No topic is set\r\n")
#define RPL_TOPIC(channel, topic) (":localhost 332 " + channel + " :" + topic + "\r\n")
#define ERR_NOCHANMODES(channel) (":localhost 477 " + channel + " :Channel doesn't support modes\r\n")

/*MODE COMMAND*/
#define RPL_CHANNELMODEIS(channel, mode, mode_params) (":localhost 324 " + channel + " " + mode + " " + mode_params + "\r\n")
#define ERR_KEYSET(channel) (":localhost 467 " + channel + " :Channel key already set\r\n")
#define ERR_UNKNOWNMODE(char, channel) (":localhost 472 " + char + " :is unknown mode char to me for " + channel + "\r\n")
#define ERR_UMODEUNKNOWNFLAG(client) (":localhost 501 " + client + " :Unknown MODE flag\r\n") // client is with prefix 

/*NICK COMMAND*/
#define ERR_ERRONEUSNICKNAME(nick) (":localhost 432 " + nick + " :Erroneous nickname\r\n")
#define ERR_NICKNAMEINUSE(nick) (":localhost 433 " + nick + " :Nickname is already in use\r\n")

/*JOIN COMMAND*/
#define ERR_CHANNELISFULL(channel) (":localhost 471 " + channel + " :Cannot join channel (+l)\r\n")
#define ERR_INVITEONLYCHAN(channel) (":localhost 473 " + channel + " :Cannot join channel (+i)\r\n")
#define ERR_BADCHANNELKEY(channel) (":localhost 475 " + channel + " :Cannot join channel (+k)\r\n")
//                 ERR_TOOMANYCHANNELS

/*OPERATOR REPLY*/
# define RPL_YOUREOPER(nick) (":localhost 381 " + nick + " :You are now an IRC operator\r\n")

#endif