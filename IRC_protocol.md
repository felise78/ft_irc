# Channels

* Channels names are strings (beginning with a '&', '#', '+' or '!' character) of length up to fifty (50) characters. Apart from the requirement that the first character is either '&', '#', '+' or '!', the only restriction on a channel name is that it SHALL NOT contain any spaces (' '), a control G (^G or ASCII 7), a comma (',').
	* Space is used as parameter separator and command is used as a list item separator by the protocol.  A colon (':') can also be used as a delimiter for the channel mask.  Channel names are case insensitive. See the protocol grammar rules (section 2.3.1) for the exact syntax of a channel name.
	* Each prefix characterizes a different channel type.

* Because of IRC's Scandinavian origin, the characters {}|^ are considered to be the lower case equivalents of the characters []\~, respectively. This is a critical issue when determining the equivalence of two nicknames or channel names.

# Messages

* Servers and clients send each other messages, which may or may not generate a reply.  If the message contains a valid command, as described in later sections, the client should expect a reply as specified but it is not advised to wait forever for the reply; client to server and server to server communication is essentially asynchronous by nature.

* Each IRC message may consist of up to three main parts: the prefix (OPTIONAL), the command, and the command parameters (maximum of fifteen (15)).  The prefix, command, and all parameters are separated by one ASCII space character (0x20) each.
	* The presence of a prefix is indicated with a single leading ASCII colon character (':', 0x3b), which MUST be the first character of the message itself.