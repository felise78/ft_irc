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
#define ERR_UNKNOWNCOMMAND(command) (":localhost 421 " + command + " :Unknown command\r\n")

/*PRIVMSG*/
#define ERR_NOSUCHNICK(user_nick, target_nick) (":localhost 401 " + user_nick + " " + target_nick + " :No such nick\r\n")
#define ERR_CANNOTSENDTOCHAN(channelName) (":localhost 404 " channelName + " :Cannot send to channel\r\n")
#define ERR_TOOMANYTARGETS(target) (":localhost 407 " + target + ":407 recipients. Use one target only.\r\n")
#define ERR_NORECIPIENT(command) (":localhost 411 :No recipient given " + command + "\r\n")
#define ERR_NOTEXTTOSEND ":localhost 412 :No text to send\r\n"
#define RPL_PRIVMSG(nick, target, message) (nick + " PRIVMSG " + target + " " + message + "\r\n") // nick is with prefix

/*KICK COMMAND*/
//ERR_NOSUCHCHANNEL(client, channel)(":localhost 403 " + client + " #" + channel + " :No such channel\r\n")
#define ERR_NOSUCHCHANNEL(nick, channelName) (":localhost 403 " + nick + " " + channelName + " :No such channel\r\n")
#define ERR_USERNOTINCHANNEL(nick, channelName) (":localhost 441 " + nick + " " + channelName + " :They aren't on that channel\r\n")
#define ERR_NOTONCHANNEL(channelName) (":localhost 442 " + channelName + " :You're not on that channel\r\n")
#define ERR_NEEDMOREPARAMS(command) (":localhost 461 " + command + " :Not enough parameters\r\n")
#define ERR_CHANOPRIVSNEEDED(channelName) (":localhost 482 " + channelName + " :You're not channel operator\r\n")
#define RPL_KICK(user_id, channelName, kicked, reason) (user_id + " KICK " + channelName + " " + kicked + " " + reason + "\r\n")

/*INVITE COMMAND*/
#define RPL_INVITE(user, invited, channelName) (user + " INVITE " + invited + " " + channelName + "\r\n")
#define RPL_INVITING(user, channelName, nick) (":localhost 341 " + user + " " + nick + " " + channelName + "\r\n")
#define ERR_USERONCHANNEL(user, channelName) (":localhost 442 " + user + " " + channelName + " :is already on channel\r\n")

/* TOPIC COMMAND: */
#define RPL_NOTOPIC(channelName) (":localhost 331 " + channelName + " :No topic is set\r\n")
#define RPL_TOPIC(nick, channelName, topic) (":localhost 332 " + nick + " " + channelName + " " + topic + "\r\n")
#define ERR_NOCHANMODES(channelName) (":localhost 477 " + channelName + " :Channel doesn't support modes\r\n")

/*MODE COMMAND*/
#define RPL_CHANNELMODEIS(channelName, mode, mode_params) (":localhost 324 " + channelName + " " + mode + " " + mode_params + "\r\n")
#define ERR_KEYSET(channelName) (":localhost 467 " + channelName + " :Channel key already set\r\n")
#define ERR_UNKNOWNMODE(char, channelName) (":localhost 472 " + char + " :is unknown mode char to me for " + channelName + "\r\n")
#define ERR_UMODEUNKNOWNFLAG(client) (":localhost 501 " + client + " :Unknown MODE flag\r\n") // client is with prefix 
// RPL_UMODEIS

/*NICK COMMAND*/
#define ERR_ERRONEUSNICKNAME(nick) (":localhost 432 " + nick + " :Erroneous nickname\r\n")
#define ERR_NICKNAMEINUSE(nick) (":localhost 433 " + nick + " :Nickname is already in use\r\n")
#define ERR_NONICKNAMEGIVEN ":localhost 431 :No nickname given\r\n"
#define RPL_NICK(old_nickname, username, new_nickname) (":" + old_nickname + "!" + username + "@localhost NICK " +  new_nickname + "\r\n")

/*JOIN COMMAND*/
#define ERR_CHANNELISFULL(channelName) (":localhost 471 " + channelName + " :Cannot join channel (+l)\r\n")
#define ERR_INVITEONLYCHAN(channelName) (":localhost 473 " + channelName + " :Cannot join channel (+i)\r\n")
#define ERR_BADCHANNELKEY(channelName) (":localhost 475 " + channelName + " :Cannot join channel (+k)\r\n")
//                 ERR_TOOMANYCHANNELS

/*PART COMMAND*/
# define RPL_PART(user_id, channelName, reason) (user_id + " PART " + channelName + " " + (reason.empty() ? "." : reason ) + "\r\n")

/*OPERATOR REPLY*/
# define RPL_YOUREOPER(nick) (":localhost 381 " + nick + " :You are now an IRC operator\r\n")
# define MODE_USERMSG(client, mode) (":" + client + " MODE " + client + " :" + mode + "\r\n")

#endif