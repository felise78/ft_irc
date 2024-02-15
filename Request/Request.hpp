#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>

typedef enum	e_commands
{
	KICK,
	INVITE,
	TOPIC,
	MODE,
	NICK,
	OPER,
	JOIN,
	PRIVMSG
}	t_commands;

class Request
{
	private:
		std::string input;
		std::string	prefix;
		std::string	command;
		std::string params;

	public:
		Request();
		~Request();
		std::string const&	getInput();
		std::string const&	getCommand();
		std::string const&	getPrefix();
		std::string const&	getParams();


};

#endif

/*
Command: PRIVMSG
Parameters: <msgtarget> <text to be sent>

Command: KICK
Parameters: <channel> *( "," <channel> ) <user> *( "," <user> )[<comment>]
The KICK command can be used to request the forced removal of a user from a channel.
It causes the <user> to PART from the <channel> by force.  For the message to be syntactically
correct, there MUST be either one channel parameter and multiple user parameter, or as many channel
parameters as there are user parameters.  If a "comment" is given, this will be sent instead of
the default message, the nickname of the user issuing the KICK.
 Examples:

   KICK &Melbourne Matthew         ; Command to kick Matthew from
                                   &Melbourne

   KICK #Finnish John :Speaking English
                                   ; Command to kick John from #Finnish
                                   using "Speaking English" as the
                                   reason (comment).

   :WiZ!jto@tolsun.oulu.fi KICK #Finnish John
                                   ; KICK message on channel #Finnish
                                   from WiZ to remove John from channel


Command: INVITE
Parameters: <nickname> <channel>
The INVITE command is used to invite a user to a channel. The parameter <nickname> is the nickname
of the person to be invited to the target channel <channel>. There is no requirement that the channel
the target user is being invited to must exist or be a valid channel. However, if the channel exists,
only members of the channel are allowed to invite other users. When the channel has invite-only flag
set, only channel operators may issue INVITE command.
Examples:

   :Angel!wings@irc.org INVITE Wiz #Dust

                                   ; Message to WiZ when he has been
                                   invited by user Angel to channel
                                   #Dust

   INVITE Wiz #Twilight_Zone       ; Command to invite WiZ to
                                   #Twilight_zone

Command: TOPIC
Parameters: <channel> [ <topic> ]
 The TOPIC command is used to change or view the topic of a channel.
   The topic for channel <channel> is returned if there is no <topic>
   given.  If the <topic> parameter is present, the topic for that
   channel will be changed, if this action is allowed for the user
   requesting it.  If the <topic> parameter is an empty string, the
   topic for that channel will be removed.
Examples:

   :WiZ!jto@tolsun.oulu.fi TOPIC #test :New topic ; User Wiz setting the
                                   topic.

   TOPIC #test :another topic      ; Command to set the topic on #test
                                   to "another topic".

   TOPIC #test :                   ; Command to clear the topic on
                                   #test.

   TOPIC #test                     ; Command to check the topic for
                                   #test.


Command: MODE
Parameters: <channel> *( ( "-" / "+" ) *<modes> *<modeparams> )
The MODE command is provided so that users may query and change the characteristics of a channel.
For more details on available modes and their uses, see "Internet Relay Chat: Channel Management"
[IRC-CHAN]. Note that there is a maximum limit of three (3) changes per command for modes that take
a parameter.
Examples:

   MODE #Finnish +imI *!*@*.fi     ; Command to make #Finnish channel
                                   moderated and 'invite-only' with user
                                   with a hostname matching *.fi
                                   automatically invited.

   MODE #Finnish +o Kilroy         ; Command to give 'chanop' privileges
                                   to Kilroy on channel #Finnish.

   MODE #Finnish +v Wiz            ; Command to allow WiZ to speak on
                                   #Finnish.

   MODE #Fins -s                   ; Command to remove 'secret' flag
                                   from channel #Fins.

   MODE #42 +k oulu                ; Command to set the channel key to
                                   "oulu".

   MODE #42 -k oulu                ; Command to remove the "oulu"
                                   channel key on channel "#42".
*/
