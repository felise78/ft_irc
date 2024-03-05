## IRC

### Introduction  
...

### How Does It Work ?  

Here is some information about `irc`, `irssi` - what it is and how it works:  
[irc wiki](https://en.wikipedia.org/wiki/IRC)   
[irssi client](https://irssi.org/New-users/)   
[list of IRC commands (wiki)](https://en.wikipedia.org/wiki/List_of_Internet_Relay_Chat_commands)  
[IRC protocol 2812](https://datatracker.ietf.org/doc/rfc2812/)  
[IRC protocol 1459](https://datatracker.ietf.org/doc/rfc1459/)  


#### How to use `netcat` to connect to an IRC server:  
`nc -v localhost 6667` - connect to the server on port 6667 and display verbose output  

At this point the server will look for the `hostname`, `NICK`, `USER` and `PASS` commands from the client.  
If those are not received the server will timeout and close the connection.  

To register the `NICK`, `USER` and `PASS` commands are required from the client.  
`PASS` - syntax:  
`PASS <password>` - Sets a connection password. This command must be sent before the NICK/USER registration combination.  

`NICK` - syntax:  
`NICK <nickname>` - Allows the client to change their IRC nickname.  

`USER` - syntax:  
`USER <username> <hostname> <servername> <realname>` - This command is used at the beginning of connection to specify the username, hostname, servername and realname of a new user.  
`<realname>` may contain spaces, and thus must be prefixed with a colon: `:real name`  


Once the client is authenticated (server received NICK, USER, PASS..) the server will send the first handshake response to the client.  
Handshake response is composed of 4 messages:  
[001 RPL_WELCOME](https://dd.ircdocs.horse/refs/numerics/001)  
[002 RPL_YOURHOST](https://dd.ircdocs.horse/refs/numerics/002)  
[003 RPL_CREATED](https://dd.ircdocs.horse/refs/numerics/003)  
[004 RPL_MYINFO](https://dd.ircdocs.horse/refs/numerics/004)  

At this point the client is authenticated and the client can request a certain action from the server with the commands.  

#### Some of the commands that can be used:  
See more here: [list of IRC commands (wiki)](https://en.wikipedia.org/wiki/List_of_Internet_Relay_Chat_commands)  
`PRIVMSG` - syntax:  
`PRIVMSG <receiver> <message>` - Sends `<message>` to `<receiver>`, which is usually a user or a channel: `.. #<channel>..`.  
This will work only if the `<receiver>` is a channel or the user that currently exists on the server.  

`LIST` by itself will list all channels on the server.  
`LIST <channel>` will list all users on the channel.  

`JOIN` - syntax:  
`JOIN #<channel>` - Joins a channel. If the channel does not exist, it will be created.  

`WHO` - syntax:  
`WHO <channel>` - Lists all users on the channel.  

`WHOIS` - syntax:  
`WHOIS <nickname>` - Returns information about the user with the nickname `<nickname>`.  

`PING` - syntax:  
`PING <server1> [<server2>]` - Tests the presence of a connection. The server will respond with a PONG message.  
The server will send a PING message to the client to test the presence of the connection:
`PING :<server1>` to this message the client should respond with `PONG :<server1>`
so the server knows the connection is still alive.  
If the client does not respond with PONG the server will close the connection within the ping timeout.  




### STANDARD C++ LIBRARY FUNCTIONS USED IN THIS PROJECT:  
[Quick Reference Functions](QUICK_REF_FUNCTIONS.md)


