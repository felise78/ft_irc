# ft_irc - Internet Relay Chat Server

A robust IRC (Internet Relay Chat) server implementation written in C++98, designed to handle multiple simultaneous client connections with full IRC protocol compliance.

## 🌟 Overview

This project implements a fully functional IRC server that can handle real IRC clients. The server supports user authentication, channel management, private messaging, and operator privileges - providing a complete IRC experience comparable to established IRC servers. Additionally, the server features an intelligent bot powered by the ChatGPT API for enhanced user interaction.

## 🚀 Features

### Core Functionality
- **Multi-client Support**: Handle multiple simultaneous connections without blocking
- **User Authentication**: Secure password-based server access
- **Channel Management**: Create, join, and manage IRC channels
- **Private Messaging**: Direct user-to-user communication
- **Operator System**: Channel operators with administrative privileges

### Supported IRC Commands
- `NICK` - Set or change nickname
- `USER` - Register user information
- `PASS` - Authenticate with server password
- `JOIN` - Join or create channels
- `PRIVMSG` - Send messages to users or channels
- `LIST` - List available channels and users
- `WHO` - Query user information
- `WHOIS` - Get detailed user information
- `PING/PONG` - Connection heartbeat

### Operator Commands
- `KICK` - Remove users from channels
- `INVITE` - Invite users to channels
- `TOPIC` - Set or view channel topics
- `MODE` - Manage channel modes:
  - `+i/-i` - Invite-only mode
  - `+t/-t` - Topic restriction mode
  - `+k/-k` - Channel password
  - `+o/-o` - Operator privileges
  - `+l/-l` - User limit

## 🛠️ Technical Implementation

### Architecture
- **Language**: C++98 compliant
- **I/O Model**: Non-blocking I/O with `poll()` (or equivalent: `select()`, `kqueue()`, `epoll()`)
- **Protocol**: TCP/IP (IPv4/IPv6)
- **Design**: Single-threaded, event-driven architecture

### Key Technical Features
- **Non-blocking Operations**: All I/O operations are non-blocking to prevent server freezing
- **Efficient Connection Management**: Single `poll()` call handles all file descriptors
- **Robust Message Parsing**: Handles partial message reception and command reconstruction
- **Memory Safe**: Proper resource management and error handling

## 📋 Requirements

### System Requirements
- C++98 compatible compiler (g++, clang++)
- POSIX-compliant operating system (Linux, macOS, BSD)
- Make utility

### Dependencies
- Standard C++ library only
- No external libraries or Boost allowed
- System networking functions (socket, bind, listen, etc.)

## 🔧 Installation & Usage

### Compilation
```bash
# Clone the repository
git clone [repository-url]
cd ft_irc

# Compile the project
make

# The executable 'ircserv' will be created
```

### Running the Server
```bash
./ircserv <port> <password>
```

**Parameters:**
- `port`: Port number for incoming connections (e.g., 6667)
- `password`: Server password required for client authentication

**Example:**
```bash
./ircserv 6667 mypassword
```

### Connecting with IRC Clients

You can connect using any standard IRC client:

#### Using irssi
```bash
irssi -c localhost -p 6667 -w mypassword
```

#### Using netcat (for testing)
```bash
nc localhost 6667
PASS mypassword
NICK mynickname
USER myuser hostname servername :Real Name
```

## 🧪 Testing

### Basic Connection Test
```bash
# Connect with netcat
nc 127.0.0.1 6667

# Send fragmented command (using Ctrl+D)
com^Dman^Dd
```

This test verifies that the server properly handles partial message reception and command reconstruction.

## 🎯 Bonus Features

Additional features that extend beyond basic IRC functionality:

- File transfer capabilities
- IRC bot integration (implemented using ChatGPT API)
- Enhanced logging system