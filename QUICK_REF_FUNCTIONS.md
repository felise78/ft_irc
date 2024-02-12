## STANDARD LIBRARIES FUNCTIONS FOR  
## INTERNET RELAY CHAT PROJECT (IRC)
QUICK GUIDE REFERENCE, EXPLANATION, USAGE EXAMPLES

1. [SOCKET](#socket)  
1.1 [STRUCT SOCKADDR](#struct-sockaddr)  
2. [CLOSE](#close)
3. [SETSOCKOPT](#setsockopt)
4. [GETSOCKNAME](#getsockname)
5. [GETPROTOBYNAME](#getprotobyname)
6. [GETHOSTBYNAME](#gethostbyname)
7. [GETADDRINFO](#getaddrinfo)
8. [FREEADDRINFO](#freeaddrinfo)
9. [BIND](#bind)
10. [CONNECT](#connect)
11. [LISTEN](#listen)
12. [ACCEPT](#accept)
13. [HTONS](#htons)
14. [HTONL](#htonl)
15. [NTOHS](#ntohs)
16. [NTOHL](#ntohl)
17. [INET_ADDR](#inet_addr)
18. [INET_NTOA](#inet_ntoa)
19. [SEND](#send)
20. [RECV](#recv)
21. [SIGNAL](#signal)
22. [SIGACTION](#sigaction)
23. [LSEEK](#lseek)
24. [FSTAT](#fstat)
25. [FCNTL](#fcntl)
26. [POLL](#poll)  
26.1 [STRUCT POLLFD](#struct-pollfd)  
27. [SELECT](#select)  
27.1 [FD_SET](#fd_set)
28. [EPOLL (epoll_create, epoll_ctl, epoll_wait)](#epoll)  
28.1 [STRUCT EPOLL_EVENT](#struct-epoll_event)  
29. [KQUEUE (kqueue, kevent)](#kqueue)   
29.1. [STRUCT KEVENT](#struct-kevent)  




## socket
1 < < < < < < < < < < < < < < < < < < < SOCKET > > > > > > > > > > > > > > > > > > > >

`socket` function in C is used to create a socket, which is an endpoint for communication between two processes.  
It is a fundamental function for network communication, used in conjunction with other functions like `bind`, `connect`, `listen`, and `accept`.  

`int socket(int domain, int type, int protocol)`  
it returns a file descriptor for the new socket, or -1 if an error occurs.  
1) `domain` specifies the communication domain for the socket. It can be either `AF_UNIX` or `AF_LOCAL` for local communication, or `AF_INET` for IPv4 network communication.
2) `type` specifies the type of socket, such as `SOCK_STREAM` for reliable TCP sockets or `SOCK_DGRAM` for UDP (datagram) sockets.
3) `protocol` specifies the protocol to be used with the socket. It can be 0 to use the default protocol for the given domain and type.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("socket");
		return 1;
	}

	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(80);
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("bind");
		return 1;
	}

	listen(sockfd, 5);

	struct sockaddr_in cli_addr;
	socklen_t clilen = sizeof(cli_addr);

	int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0) {
		perror("accept");
		return 1;
	}

	// Now newsockfd can be used to communicate with the client

	return 0;
}
```
The example above creates a TCP socket, binds it to available IP address on port 80, and listens for incoming connections.  
When a client connects, a new socket is created for communication with the client.  
</details>


## struct sockaddr
1.1 < < < < < < < < < < < < < < < < < < < STRUCT SOCKADDR > > > > > > > > > > > > > > > > > > > >

`struct sockaddr` and `struct sockaddr_in` are structures used in socket programming for handling internet addresses.

`struct sockaddr` is a generic socket address structure, used by functions like `bind`, `connect`, `accept`, and `getsockname`.

```c
struct sockaddr {
	unsigned short	sa_family;		// Address family (AF_INET or AF_UNIX)
	char			sa_data[14];	// Protocol-specific address
};
```
- `sa_family` is the address family. It can be either `AF_INET` for IPv4 addresses or `AF_UNIX` for local addresses.
- `sa_data` is the protocol-specific address. It can be a 14-byte IPv4 address or a 14-byte path to a local socket.

`struct sockaddr_in` is a structure used to specify an IPv4 address.
```c
struct sockaddr_in {
	short			sin_family;		// Address family (AF_INET)
	unsigned short	sin_port;		// Port number
	struct in_addr	sin_addr;		// Internet address
	char 			sin_zero[8];	// Unused
};
```
- `sin_family` is the address family. It is always set to `AF_INET`.
- `sin_port` is the port number.
- `sin_addr` is the internet address.
- `sin_zero` is unused and should be set to all zeros.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("socket");
		return 1;
	}

	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(80);
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("bind");
		return 1;
	}

	listen(sockfd, 5);

	struct sockaddr_in cli_addr;
	socklen_t clilen = sizeof(cli_addr);

	int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0) {
		perror("accept");
		return 1;
	}

	// Now newsockfd can be used to communicate with the client

	return 0;
}
```
The example above creates a TCP socket, binds it to available IP address on port 80, and listens for incoming connections.  
When a client connects, a new socket is created for communication with the client.  
</details>




## close
2 < < < < < < < < < < < < < < < < < < < CLOSE > > > > > > > > > > > > > > > > > > > >

`close` function in C is used to close a file descriptor. It is typically used in  
conjunction with the `open` function to close a file after it has been opened.  
 
`int close(int fd)`  
it returns 0 on success, or -1 if an error occurs.  
1) `fd` is the file descriptor to close.  
 



## setsockopt
3 < < < < < < < < < < < < < < < < < < < SETSOCKOPT > > > > > > > > > > > > > > > > > > > >

`setsockopt` function in C is used to set socket options.  
It is typically used in conjunction with the `socket` function to set socket options before binding the socket.  
Options may exist at multiple protocol levels, but they are always present at the uppermost socket level.  

`int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen)`  
it returns 0 on success, or -1 if an error occurs.
1) `sockfd` is the file descriptor returned by `socket`.
2) `level` specifies the protocol level at which the option resides. It can be one of the following values:
	- `SOL_SOCKET` - Socket level
	- `IPPROTO_IP` - IP level
	- `IPPROTO_IPV6` - IPv6 level
	- `IPPROTO_TCP` - TCP level
	- `IPPROTO_UDP` - UDP level
3) `optname` specifies the option to set. It can be one of the following values:
	- `SO_ACCEPTCONN` - Accept connections on socket
	- `SO_BROADCAST` - Allow transmission of broadcast messages
	- `SO_DEBUG` - Record debugging information
	- `SO_DONTROUTE` - Bypass routing table
	- `SO_KEEPALIVE` - Keep connection alive
	- `SO_LINGER` - Linger on close if data is present
	- `SO_OOBINLINE` - Receive out-of-band data in band
	- `SO_RCVBUF` - Receive buffer size
	- `SO_RCVLOWAT` - Receive low-water mark
	- `SO_RCVTIMEO` - Receive timeout
	- `SO_REUSEADDR` - Allow reuse of local addresses
	- `SO_SNDBUF` - Send buffer size
	- `SO_SNDLOWAT` - Send low-water mark
	- `SO_SNDTIMEO` - Send timeout
	- `SO_TYPE` - Get socket type
4) `optval` is a pointer to a buffer containing the value of the option to set.
5) `optlen` is the size of the buffer.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

int main() {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("socket");
		return 1;
	}

	int enable = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
		perror("setsockopt");
		return 1;
	}

	// Now the socket can be used with the SO_REUSEADDR option enabled

	return 0;
}
```
The example above creates a TCP socket and sets the `SO_REUSEADDR` option on the socket.  
This option allows the socket to reuse the address and port that is in the `TIME_WAIT` state.  
This can be useful in a server that has been restarted and needs to bind to the same address and port.  
</details>




## getsockname
4 < < < < < < < < < < < < < < < < < < < GETSOCKNAME > > > > > > > > > > > > > > > > > > > >

`getsockname` function in C is used to retrieve the local name (address and port) of a socket.  
This is useful when you want to know the address and port assigned to a socket by the system when you did not bind the socket manually.  

`int getsockname(int sockfd, struct sockaddr *addr, socklen_t *addrlen)`  
it returns 0 on success, or -1 if an error occurs.  
1) `sockfd` is the file descriptor returned by `socket`.  
2) `addr` is a pointer to a `struct sockaddr` structure that will store the local address.  
3) `addrlen` is a pointer to a `socklen_t` variable that will store the size of the `struct sockaddr` structure.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("socket");
		return 1;
	}

	struct sockaddr_in serv_addr;
	socklen_t addrlen = sizeof(serv_addr);

	if (getsockname(sockfd, (struct sockaddr *) &serv_addr, &addrlen) < 0) {
		perror("getsockname");
		return 1;
	}

	printf("Local IP address is: %s\n", inet_ntoa(serv_addr.sin_addr));
	printf("Local port is: %d\n", (int) ntohs(serv_addr.sin_port));

	return 0;
}
```
The example above creates a TCP socket and retrieves the local address and port assigned to the socket by the system.  
- `getsockname` function is used to retrieve the local address and port assigned to a socket.  
- `inet_ntoa` function is used to convert the binary representation of the IP address to a string in dotted-decimal notation.  
- `ntohs` function is used to convert the port number from network byte order to host byte order.  
</details>




## getprotobyname
5 < < < < < < < < < < < < < < < < < < < GETPROTOBYNAME > > > > > > > > > > > > > > > > > > > >

`getprotobyname` function in C is used to retrieve the protocol information corresponding to a protocol name.  
It is typically used in conjunction with the `socket` function to specify the protocol to use when creating a socket.  

`struct protoent *getprotobyname(const char *name)`  
it returns a pointer to a `struct protoent` structure on success, or NULL if an error occurs.
1) `name` is a pointer to a string containing the protocol name.

```c
struct protoent {
		char	*p_name;		// Official protocol name
		char	**p_aliases;	// Alias list
		int		p_proto;		// Protocol number
};
```
- `p_name` is a pointer to a string containing the official protocol name.
- `p_aliases` is a pointer to a NULL-terminated list of alternate names.
- `p_proto` is the protocol number.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <netdb.h>

int main() {
	struct protoent *pe;

	pe = getprotobyname("tcp");
	if (pe) {
		printf("Name: %s, Protocol number: %d\n", pe->p_name, pe->p_proto);
	} else {
		printf("Protocol not found\n");
	}

	return 0;
}
```
The example above retrieves the protocol information for the TCP protocol.
The `getprotobyname` function is used to retrieve the protocol information for the TCP protocol.
The `struct protoent` structure contains the protocol name and protocol number.
</details>




## gethostbyname
6 < < < < < < < < < < < < < < < < < < < GETHOSTBYNAME > > > > > > > > > > > > > > > > > > > >

`gethostbyname` function in C is used to retrieve the host information corresponding to a host name.
It is typically used in conjunction with the `socket` function to specify the address of the remote host when creating a socket.

`struct hostent *gethostbyname(const char *name)`
it returns a pointer to a `struct hostent` structure on success, or NULL if an error occurs.
1) `name` is a pointer to a string containing the host name.

```c
struct hostent {
	char	*h_name;		// Official name of the host
	char	**h_aliases;	// Alias list
	int		h_addrtype;		// Host address type
	int		h_length;		// Length of address
	char	**h_addr_list;	// List of addresses
};
```
- `h_name` is a pointer to a string containing the official name of the host.
- `h_aliases` is a pointer to a NULL-terminated list of alternate names.
- `h_addrtype` is the address type. It can be either `AF_INET` for IPv4 addresses or `AF_INET6` for IPv6 addresses.
- `h_length` is the length of the address in bytes.
- `h_addr_list` is a pointer to a NULL-terminated list of addresses.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <netdb.h>

int main() {
	struct hostent *he;

	he = gethostbyname("www.example.com");
	if (he) {
		printf("Official name: %s\n", he->h_name);
		printf("Address type: %s\n", (he->h_addrtype == AF_INET) ? "AF_INET" : "AF_INET6");
		printf("Address length: %d\n", he->h_length);
		printf("Addresses: ");
		for (int i = 0; he->h_addr_list[i] != NULL; i++) {
			printf("%s ", inet_ntoa(*(struct in_addr *) he->h_addr_list[i]));
		}
		printf("\n");
	} else {
		printf("Host not found\n");
	}

	return 0;
}
```
The example above retrieves the host information for the host www.example.com.
The `gethostbyname` function is used to retrieve the host information for the host www.example.com.
The `struct hostent` structure contains the official name of the host, the address type, the address length, and a list of addresses.
The `inet_ntoa` function is used to convert the binary representation of the IP address to a string in dotted-decimal notation.
</details>




## getaddrinfo
7 < < < < < < < < < < < < < < < < < < < GET ADDR INFO > > > > > > > > > > > > > > > > > > > >

`getaddrinfo` function in C is used to resolve the network address (both service and protocol) into a list of address structures that can be used later for socket creation and binding.  
It is a modern replacement for the `gethostbyname` and `gethostbyaddr` functions.  

`int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res)`  
it returns 0 on success, or a non-zero error code if an error occurs.
1) `node` is a pointer to a string containing the network address (either a hostname or an IP address) to resolve.
2) `service` is a pointer to a string containing the service name or port number to resolve.
3) `hints` is a pointer to a `struct addrinfo` structure that specifies the preferred socket type and protocol.
4) `res` is a pointer to a pointer to a `struct addrinfo` structure that will store the result of the address resolution.

```c
struct addrinfo {
	int				ai_flags;		// Input flags
	int				ai_family;		// Address family
	int				ai_socktype;	// Socket type
	int				ai_protocol;	// Protocol
	socklen_t		ai_addrlen;		// Length of address
	struct sockaddr	*ai_addr;		// Address
	char 			*ai_canonname;	// Canonical name
	struct addrinfo	*ai_next;		// Next node
};
```
- `ai_flags` is a bit mask specifying input flags. It can be one or more of the following values OR-ed together:
	- `AI_ADDRCONFIG` - Only return IPv4 addresses if an IPv4 address is configured.
	- `AI_ALL` - Return all matching addresses.
	- `AI_CANONNAME` - Fill in the `ai_canonname` field of the `struct addrinfo` structure.
	- `AI_NUMERICHOST` - Return only numeric host addresses.
	- `AI_NUMERICSERV` - Return only numeric service addresses.
	- `AI_PASSIVE` - Return an address that can be used to bind to a socket for accepting incoming connections.
	- `AI_V4MAPPED` - If no IPv6 addresses are found, return IPv4-mapped IPv6 addresses.
- `ai_family` is the address family. It can be either `AF_INET` for IPv4 addresses or `AF_INET6` for IPv6 addresses.
- `ai_socktype` is the socket type. It can be either `SOCK_STREAM` for TCP sockets or `SOCK_DGRAM` for UDP sockets.
- `ai_protocol` is the protocol. It can be either `IPPROTO_TCP` for TCP sockets or `IPPROTO_UDP` for UDP sockets.
- `ai_addrlen` is the length of the `ai_addr` field.
- `ai_addr` is a pointer to a `struct sockaddr` structure that contains the address.
- `ai_canonname` is a pointer to a string containing the canonical name of the host.
- `ai_next` is a pointer to the next node in a linked list of `struct addrinfo` structures.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main() {
	struct addrinfo hints, *res;
	int status;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // AF_INET or AF_INET6 to force version
	hints.ai_socktype = SOCK_STREAM;

	if ((status = getaddrinfo("www.example.com", "80", &hints, &res)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		return 1;
	}

	// res now points to a linked list of 1 or more struct addrinfos

	// ... do everything until you don't need res anymore ....

	freeaddrinfo(res); // free the linked list

	return 0;
}
```
The example above resolves the address of the host www.example.com on port 80.  
The `hints` structure is used to specify that we are looking for a stream (TCP) socket.  
If tha call to `getaddrinfo` is successful, the `res` pointer will point to a linked list of `struct addrinfo` structures that contain the resolved addresses.  
</details>




## freeaddrinfo
8 < < < < < < < < < < < < < < < < < < < FREE ADDR INFO > > > > > > > > > > > > > > > > > > > >

`freeaddrinfo` function in C is used to free the dynamically allocated memory of the linked list of `struct addrinfo` structures returned by the `getaddrinfo` function.  

`void freeaddrinfo(struct addrinfo *res)`  
1) `res` is a pointer to the first node in the linked list of `struct addrinfo` structures.

see example 7 [(getaddrinfo)](#getaddrinfo)




## bind
9 < < < < < < < < < < < < < < < < < < < BIND > > > > > > > > > > > > > > > > > > > >

`bind` function in C is used to associate a socket with a local address.  
It is used in conjunction with the `socket` function to create a socket.  

`int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)`  
it returns 0 on success, or -1 if an error occurs.  
1) `sockfd` is the file descriptor returned by `socket`.  
2) `addr` is a pointer to a `struct sockaddr` structure that contains the local address to associate with the socket.  
3) `addrlen` is the size of the `struct sockaddr` structure.  

<details>
<summary>EXAMPLE:</summary>

```c
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("socket");
		return 1;
	}

	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(80);
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("bind");
		return 1;
	}

	// Now sockfd can be used to listen for connections

	return 0;
}
```
The example above creates a TCP socket and binds it to all available IP addresses on port 80.  

also see examples 1 [(socket)](#socket) and 1.1 [(struct sockaddr)](#struct-sockaddr)
</details>




## connect
10 < < < < < < < < < < < < < < < < < < < CONNECT > > > > > > > > > > > > > > > > > > > >

`connect` function in C is used in socket programming to connect to a remote host.  
It is typically used in clients that connect to a server.  

`int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen)`  
it returns 0 on success, or -1 if an error occurs.  
1) `sockfd` is the file descriptor returned by `socket`.  
2) `addr` is a pointer to a `struct sockaddr` structure that contains the address of the  
remote host to connect to.  
3) `addrlen` is the size of the `struct sockaddr` structure.  

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("socket");
		return 1;
	}

	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(80);
	serv_addr.sin_addr.s_addr = inet_addr("93.184.216.34"); // example.com

	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("connect");
		return 1;
	}

	// Now sockfd can be used to communicate with the server

	return 0;
}
```
The example above creates a TCP socket and connects to the address 93.184.216.34 (example.com) on port 80.  
If the connection is successful, the socket can be used to communicate with the server.  
</details>




## listen
11 < < < < < < < < < < < < < < < < < < < LISTEN > > > > > > > > > > > > > > > > > > > >

`listen` function in C is used in socket programming to mark a socket as a passive socket, which is a socket that will be used to accept incoming connection requests using the `accept` function.  

`int listen(int sockfd, int backlog)`  
it returns 0 on success, or -1 if an error occurs.  
1) `sockfd` is the file descriptor returned by `socket`.
2) `backlog` is the maximum number of pending connections that can be queued up before connections are refused.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("socket");
		return 1;
	}

	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(80);
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("bind");
		return 1;
	}

	if (listen(sockfd, 5) < 0) {
		perror("listen");
		return 1;
	}

	// Now sockfd can be used to accept connections

	return 0;
}
```
The example above creates a TCP socket, binds it to all available IP addresses on port 80, and sets it to listen for incoming connections.  
`listen` function is used to mark the socket as a passive socket with a backlog of 5, which means that up to 5 clients can be waiting for a connection at any given time.
</details>




## accept
12 < < < < < < < < < < < < < < < < < < < ACCEPT > > > > > > > > > > > > > > > > > > > >

`accept` function in C is used in socket programming to accept a new connection on a
socket. It is typically used in servers that listen for incoming client connections.

`int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)` it returns a file
descriptor for the new socket, or -1 if an error occurs.
- `sockfd` is the file descriptor returned by `socket`.
- `addr` is a pointer to a `struct sockaddr` structure that will store the address of
the client that connects to the socket.
- `addrlen` is a pointer to a `socklen_t` variable that will store the size of the
`struct sockaddr` structure.

see example 1 [(socket)](#socket) and 1.1 [(struct sockaddr)](#struct-sockaddr)




## htons
13 < < < < < < < < < < < < < < < < < < < HTONS > > > > > > > > > > > > > > > > > > > >

`htons` function in C is used to convert a 16-bit integer (short) from host byte order
to network byte order. `htons` stands for "host to network short".

`uint16_t htons(uint16_t hostshort)`  
it returns the converted value.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <arpa/inet.h>

int main() {
	uint16_t host_short = 3000;
	uint16_t network_short = htons(host_short);

	printf("Host byte order: %u\n", host_short);
	printf("Network byte order: %u\n", network_short);

	return 0;
}
```
The output of the program is:  
Host byte order: 3000  
Network byte order: 10752  
</details>




## htonl
14 < < < < < < < < < < < < < < < < < < < HTONL > > > > > > > > > > > > > > > > > > > >

`htonl` function in C is used to convert a 32-bit integer (long) from host byte order
to network byte order. `htonl` stands for "host to network long".  

`uint32_t htonl(uint32_t hostlong)`  
it returns the converted value.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <arpa/inet.h>

int main() {
	uint32_t host_long = 3000000;
	uint32_t network_long = htonl(host_long);

	printf("Host byte order: %u\n", host_long);
	printf("Network byte order: %u\n", network_long);

	return 0;
}
```
The output of the program is:  
Host byte order: 3000000  
Network byte order: 50331648  
</details>




## ntohs
15 < < < < < < < < < < < < < < < < < < < NTOHS > > > > > > > > > > > > > > > > > > > >

`ntohs` function in C is used to convert a 16-bit integer (short) from network byte order
to host byte order. `ntohs` stands for "network to host short".

`uint16_t ntohs(uint16_t netshort)`  
it returns the converted value.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <arpa/inet.h>

int main() {
	uint16_t network_short = 10752;
	uint16_t host_short = ntohs(network_short);

	printf("Network byte order: %u\n", network_short);
	printf("Host byte order: %u\n", host_short);

	return 0;
}
```
The output of the program is:  
Network byte order: 10752  
Host byte order: 3000  
</details>




## ntohl
16 < < < < < < < < < < < < < < < < < < < NTOHL > > > > > > > > > > > > > > > > > > > >

`ntohl` function in C is used to convert a 32-bit integer (long) from network byte order
to host byte order. `ntohl` stands for "network to host long".  

`uint32_t ntohl(uint32_t netlong)`  
it returns the converted value.  

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <arpa/inet.h>

int main() {
	uint32_t network_long = 50331648;
	uint32_t host_long = ntohl(network_long);

	printf("Network byte order: %u\n", network_long);
	printf("Host byte order: %u\n", host_long);

	return 0;
}
```
The output of the program is:  
Network byte order: 50331648  
Host byte order: 3000000  
</details>




## inet_addr
17 < < < < < < < < < < < < < < < < < < < INET_ADDR > > > > > > > > > > > > > > > > > > > >

`inet_addr` function in C is used to convert an IPv4 address in dotted-decimal notation string format to its binary representation that can be used as an argument for functions like `connect` and `bind`.  

`in_addr_t inet_addr(const char *cp)`  
it returns the binary representation of the IPv4 address, or -1 if an error occurs.
1) `cp` is a pointer to a string containing the IPv4 address in dotted-decimal notation.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
	const char *ip_str = "93.184.216.34"; // example.com
	in_addr_t ip = inet_addr(ip_str);

	if (ip == INADDR_NONE) {
		printf("Invalid IP address: %s\n", ip_str);
		return 1;
	}

	printf("IP in network byte order: %u\n", ip);

	return 0;
}
```
`in_addr_t` is a typedef for `uint32_t` and is defined in the `netinet/in.h` header file.  
The output of the program is: `IP in network byte order: 570666757`  
</details>




## inet_ntoa
18 < < < < < < < < < < < < < < < < < < < INET_NTOA > > > > > > > > > > > > > > > > > > > >

`inet_ntoa` function in C is used to convert the binary representation of an IPv4 address to a string in dotted-decimal notation.

`char *inet_ntoa(struct in_addr in)`
it returns a pointer to a string containing the dotted-decimal notation of the IPv4 address.
1) `in` is a `struct in_addr` structure containing the binary representation of the IPv4 address.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void print_binary(unsigned int number) {
	for (int i = 7; i >= 0; --i) {
		putc((number & (1 << i)) ? '1' : '0', stdout);
	}
}

int main() {
	struct in_addr ip;

	ip.s_addr = inet_addr("127.0.0.1");

	for (int i = 24; i >= 0; i -= 8) {
		print_binary((ip.s_addr >> i) & 0xFF);
		printf(i ? "." : "\n");
	}
	printf("\n");

	char *ip_str = inet_ntoa(ip);
	if (ip_str) {
		printf("IP in dotted-decimal notation: %s\n", ip_str);
	} else {
		printf("Invalid IP address\n");
	}
	printf("\n");

	return 0;
}

```

The output of the program is: 
`
00000001.00000000.00000000.01111111

IP in dotted-decimal notation: 127.0.0.1
`
</details>




## send
19 < < < < < < < < < < < < < < < < < < < SEND > > > > > > > > > > > > > > > > > > > >

`send` function in C is used in socket programming to send data on a socket.  
It is typically used in clients that send data to a server.  

`ssize_t send(int sockfd, const void *buf, size_t len, int flags)`  
it returns the number of bytes sent, or -1 if an error occurs.
1) `sockfd` is the file descriptor returned by `socket`.
2) `buf` is a pointer to the data to send.
3) `len` is the size of the data to send.
4) `flags` specifies the type of message transmission. It can be 0 or more of the following values OR-ed together:
	- `MSG_DONTROUTE` - Send without using a routing table.
	- `MSG_DONTWAIT` - Send without blocking.
	- `MSG_EOR` - Indicate end of record.
	- `MSG_OOB` - Send out-of-band data.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main() {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("socket");
		return 1;
	}

	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(80);
	serv_addr.sin_addr.s_addr = inet_addr("93.184.216.34"); // example.com

	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("connect");
		return 1;
	}

	const char *message = "Hello, World!";
	if (send(sockfd, message, strlen(message), 0) < 0) {
		perror("send");
		return 1;
	}

	// Now sockfd can be used to receive data from the server, see example 22

	return 0;
}
```
The example above creates a TCP socket and connects to the server at IP address 93.184.216.34 (example.com) on port 80.  
It then sends the message "Hello, World!" to the server.  
If the send operation is successful, the socket can be used to receive data from the server.  
</details>




## recv
20 < < < < < < < < < < < < < < < < < < < RECV > > > > > > > > > > > > > > > > > > > >

`recv` function in C is used in socket programming to receive data from a socket.  
It is typically used in servers that receive data from clients.  

`ssize_t recv(int sockfd, void *buf, size_t len, int flags)`  
it returns the number of bytes received, or -1 if an error occurs.  
1) `sockfd` is the file descriptor returned by `socket`.
2) `buf` is a pointer to the buffer where the data will be stored.
3) `len` is the size of the buffer.
4) `flags` specifies the type of message reception. It can be 0 or more of the following values OR-ed together:
	- `MSG_DONTWAIT` - Receive without blocking.
	- `MSG_OOB` - Receive out-of-band data.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main() {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("socket");
		return 1;
	}

	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(80);
	serv_addr.sin_addr.s_addr = inet_addr("93.184.216.34"); // example.com

	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("connect");
		return 1;
	}

	const char *message = "Hello, World!";
	if (send(sockfd, message, strlen(message), 0) < 0) {
		perror("send");
		return 1;
	}

	char buffer[256];
	int n = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
	if (n < 0) {
		perror("recv");
		return 1;
	}

	buffer[n] = '\0';
	printf("Received: %s\n", buffer);

	return 0;
}
```
The example above creates a TCP socket and connects to the server at IP address 93.184.216.34 (example.com) on port 80.  
It then sends the message "Hello, World!" to the server and receives a response from the server.  
The `recv` function is used to receive the response from a server.  
The response is stored in a buffer and printed to the standard output.  
</details>




## signal
21 < < < < < < < < < < < < < < < < < < < SIGNAL > > > > > > > > > > > > > > > > > > > >

`signal` function in C is used to change the action taken by a process on receipt of a signal.  
It can be used to define a custom handler function that will be called when the process 
receives a specific signal.  

`void (*signal(int sig, void (*func)(int)))(int)`  
it returns a pointer to the previous handler function, or `SIG_ERR` if an error occurs.  
1) `sig` is the signal to handle. It can be one of the following values:  
	- `SIGABRT` - Abort signal  
	- `SIGALRM` - Alarm clock  
	- `SIGBUS` - Access to an undefined portion of a memory object  
	- `SIGCHLD` - Child process terminated, stopped, or continued  
	- `SIGCONT` - Continue executing, if stopped  
	- `SIGFPE` - Erroneous arithmetic operation  
	- `SIGHUP` - Hangup  
	- `SIGILL` - Illegal instruction  
	- `SIGINT` - Terminal interrupt signal  
	- `SIGKILL` - Kill (cannot be caught or ignored)  
	- `SIGPIPE` - Write on a pipe with no one to read it  
	- `SIGQUIT` - Terminal quit signal  
	- `SIGSEGV` - Invalid memory reference  
	- `SIGSTOP` - Stop executing (cannot be caught or ignored)  
	- `SIGTERM` - Termination signal  
	- `SIGTSTP` - Terminal stop signal  
	- `SIGTTIN` - Background process attempting read  
	- `SIGTTOU` - Background process attempting write  
	- `SIGUSR1` - User-defined signal 1  
	- `SIGUSR2` - User-defined signal 2  
	- `SIGPOLL` - Pollable event  
	- `SIGPROF` - Profiling timer expired  
	- `SIGSYS` - Bad system call  
	- `SIGTRAP` - Trace/breakpoint trap  
	- `SIGURG` - High bandwidth data is available at a socket  
	- `SIGVTALRM` - Virtual timer expired  
	- `SIGXCPU` - CPU time limit exceeded  
	- `SIGXFSZ` - File size limit exceeded  
2) `func` is a pointer to the handler function to call when the process receives the signal.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void sigint_handler(int sig_num) {
	printf("Received SIGINT, exiting...\n");
	exit(0);
}

int main() {
	signal(SIGINT, sigint_handler);

	printf("Press Ctrl+C to exit...\n");

	while(1) {
		sleep(1);
	}

	return 0;
}
```
The example above defines a custom handler function for the `SIGINT` signal 
using the `signal` function.  
`signal_handler` function is set to be called when the process receives the `SIGINT` signal,
(usually when the user presses Ctrl+C).  
The program then enters an infinite loop, and can be terminated by pressing Ctrl+C.  
</details>




## sigaction
22 < < < < < < < < < < < < < < < < < < < SIGACTION > > > > > > > > > > > > > > > > > > > >

`sigaction` function in C is used to change the action taken by a process on receipt of a signal.
It can be used to define a custom handler function that will be called when the process
receives a specific signal.

`int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact)`
it returns 0 on success, or -1 if an error occurs.

1) `signum` is the signal to handle. It can be one of the following values:
	- `SIGABRT` - Abort signal
	- `SIGALRM` - Alarm clock
	- `SIGBUS` - Access to an undefined portion of a memory object
	- `SIGCHLD` - Child process terminated, stopped, or continued
	- `SIGCONT` - Continue executing, if stopped
	- `SIGFPE` - Erroneous arithmetic operation
	- `SIGHUP` - Hangup
	- `SIGILL` - Illegal instruction
	- `SIGINT` - Terminal interrupt signal
	- `SIGKILL` - Kill (cannot be caught or ignored)
	- `SIGPIPE` - Write on a pipe with no one to read it
	- `SIGQUIT` - Terminal quit signal
	- `SIGSEGV` - Invalid memory reference
	- `SIGSTOP` - Stop executing (cannot be caught or ignored)
	- `SIGTERM` - Termination signal
	- `SIGTSTP` - Terminal stop signal
	- `SIGTTIN` - Background process attempting read
	- `SIGTTOU` - Background process attempting write
	- `SIGUSR1` - User-defined signal 1
	- `SIGUSR2` - User-defined signal 2
	- `SIGPOLL` - Pollable event
	- `SIGPROF` - Profiling timer expired
	- `SIGSYS` - Bad system call
	- `SIGTRAP` - Trace/breakpoint trap
	- `SIGURG` - High bandwidth data is available at a socket
	- `SIGVTALRM` - Virtual timer expired
	- `SIGXCPU` - CPU time limit exceeded
	- `SIGXFSZ` - File size limit exceeded
2) `act` is a pointer to a `struct sigaction` structure that specifies the action to take when the signal is received.
3) `oldact` is a pointer to a `struct sigaction` structure that will store the previous action taken by the process on receipt of the signal.

```c
struct sigaction {
	void (*sa_handler)(int); // Pointer to a signal-catching function or one of the following values:
		// SIG_DFL - Default action
		// SIG_IGN - Ignore the signal
	void (*sa_sigaction)(int, siginfo_t *, void *); // Pointer to a signal-catching function
	sigset_t sa_mask; // Set of signals to be blocked during execution of the signal-catching function
	int sa_flags; // Special flags
	void (*sa_restorer)(void); // Not used
};
```
- `sa_handler` is a pointer to a signal-catching function or one of the following values:
	- `SIG_DFL` - Default action
	- `SIG_IGN` - Ignore the signal
- `sa_sigaction` is a pointer to a signal-catching function.
- `sa_mask` is a set of signals to be blocked during the execution of the signal-catching function.
- `sa_flags` is a set of special flags.
- `sa_restorer` is not used.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void sigint_handler(int sig_num) {
	printf("Received SIGINT, exiting...\n");
	exit(0);
}

int main() {
	struct sigaction sa;
	sa.sa_handler = sigint_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	if (sigaction(SIGINT, &sa, NULL) < 0) {
		perror("sigaction");
		return 1;
	}

	printf("Press Ctrl+C to exit...\n");

	while(1) {
		sleep(1);
	}

	return 0;
}
```
The example above defines a custom handler function for the `SIGINT` signal using the `sigaction` function.
`sigint_handler` function is set to be called when the process receives the `SIGINT` signal (usually when the user presses Ctrl+C).
The program then enters an infinite loop, and can be terminated by pressing Ctrl+C.
</details>




## lseek
23 < < < < < < < < < < < < < < < < < < < LSEEK > > > > > > > > > > > > > > > > > > > >

`lseek` function in C is used to change the file offset of an open file.

`off_t lseek(int fd, off_t offset, int whence)`
it returns the new file offset, or -1 if an error occurs.
1) `fd` is the file descriptor of the open file.
2) `offset` is the number of bytes to move the file offset.
3) `whence` specifies the starting point for the file offset. It can be one of the following values:
	- `SEEK_SET` - Set the file offset to `offset` bytes.
	- `SEEK_CUR` - Set the file offset to the current offset plus `offset` bytes.
	- `SEEK_END` - Set the file offset to the size of the file plus `offset` bytes.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
	int fd = open("test.txt", O_RDONLY);
	if (fd < 0) {
		perror("open");
		return 1;
	}

	off_t offset = lseek(fd, 0, SEEK_END);
	if (offset < 0) {
		perror("lseek");
		return 1;
	}

	printf("File size: %ld bytes\n", offset);

	close(fd);

	return 0;
}
```
The example above opens a file and retrieves the size of the file using the `lseek` function.
The file offset is set to the end of the file, and the current file offset is returned.
</details>




## fstat
24 < < < < < < < < < < < < < < < < < < < FSTAT > > > > > > > > > > > > > > > > > > > >

`fstat` function in C is used to retrieve information about an open file.

`int fstat(int fd, struct stat *buf)`
it returns 0 on success, or -1 if an error occurs.
1) `fd` is the file descriptor of the open file.
2) `buf` is a pointer to a `struct stat` structure that will store the file information.

```c
struct stat {
	dev_t st_dev; // ID of device containing file
	ino_t st_ino; // Inode number
	mode_t st_mode; // File type and mode
	nlink_t st_nlink; // Number of hard links
	uid_t st_uid; // User ID of owner
	gid_t st_gid; // Group ID of owner
	dev_t st_rdev; // Device ID (if special file)
	off_t st_size; // Total size, in bytes
	blksize_t st_blksize; // Block size for file system I/O
	blkcnt_t st_blocks; // Number of 512B blocks allocated
	time_t st_atime; // Time of last access
	time_t st_mtime; // Time of last modification
	time_t st_ctime; // Time of last status change
};
```

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main() {
	int fd = open("test.txt", O_RDONLY);
	if (fd < 0) {
		perror("open");
		return 1;
	}

	struct stat buf;
	if (fstat(fd, &buf) < 0) {
		perror("fstat");
		return 1;
	}

	printf("File size: %ld bytes\n", buf.st_size);

	close(fd);

	return 0;
}
```
The example above opens a file and retrieves the size of the file using the `fstat` function.
The file information is stored in a `struct stat` structure, and the file size is printed to the standard output.
</details>




## fcntl
25 < < < < < < < < < < < < < < < < < < < FCNTL > > > > > > > > > > > > > > > > > > > >

`fcntl` function in C is used to perform various operations on files or file descriptors.  
It can be used to get or set the file attributes, control locks on files, and more.  

`int fcntl(int fd, int cmd, ...)`  
it returns the result of the operation, or -1 if an error occurs.  
1) `fd` is the file descriptor to operate on.  
2) `cmd` is the operation to perform. It can be one of the following values:  
	- `F_DUPFD` - Duplicate file descriptor  
	- `F_GETFD` - Get file descriptor flags  
	- `F_SETFD` - Set file descriptor flags  
	- `F_GETFL` - Get file status flags  
	- `F_SETFL` - Set file status flags  
	- `F_GETLK` - Get record locking information  
	- `F_SETLK` - Set record locking information  
	- `F_SETLKW` - Set record locking information; wait if blocked  
	- `F_GETOWN` - Get process or process group ID to receive SIGURG signals  
	- `F_SETOWN` - Set process or process group ID to receive SIGURG signals  
	- `F_GETLK64` - Get record locking information  
	- `F_SETLK64` - Set record locking information  
	- `F_SETLKW64` - Set record locking information; wait if blocked  
	- `F_SETOWN_EX` - Set process or process group ID to receive SIGURG signals  
	- `F_GETOWN_EX` - Get process or process group ID to receive SIGURG signals  
	- `F_GETOWNER_UIDS` - Get owner UID and filesystem UID  
	- `F_OFD_GETLK` - Get record locking information  
	- `F_OFD_SETLK` - Set record locking information  
	- `F_OFD_SETLKW` - Set record locking information; wait if blocked  
	- `F_DUPFD_CLOEXEC` - Duplicate file descriptor with close-on-exit set  
	- `F_SETLEASE` - Set a lease  
	- `F_GETLEASE` - Get a lease  
	- `F_NOTIFY` - Request notifications on a directory  
	- `F_SETPIPE_SZ` - Set pipe page size array  
	- `F_GETPIPE_SZ` - Get pipe page size array  
	- `F_ADD_SEALS` - Add seals  
	- `F_GET_SEALS` - Get seals  
	- `F_GET_RW_HINT` - Get read/write hints  
	- `F_SET_RW_HINT` - Set read/write hints  
	- `F_GET_FILE_RW_HINT` - Get file read/write hints  
	- `F_SET_FILE_RW_HINT` - Set file read/write hints  

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
	int fd = open("test.txt", O_RDONLY);
	if (fd < 0) {
		perror("open");
		return 1;
	}

	int flags = fcntl(fd, F_GETFL);
	if (flags < 0) {
		perror("fcntl");
		return 1;
	}

	printf("File status flags: %o\n", flags);

	close(fd);

	return 0;
}
```
The example above opens a file and retrieves the file status flags using the `fcntl` function.  
The flags are printed in octal format.  
</details>




## poll
26 < < < < < < < < < < < < < < < < < < < POLL > > > > > > > > > > > > > > > > > > > >

`poll` function in C is used to monitor multiple file descriptors to see if they are
ready for reading, writing, or if an exceptional condition (error) has occurred.  
It is similar to the `select` function, but it is more flexible and scalable, it
doesn't require recalculating the maximum file descriptor value, and can monitor more
than `FD_SETSIZE` (1024) file descriptors.  

`int poll(struct pollfd *fds, nfds_t nfds, int timeout)`  
Returns the number of file descriptors that are ready for reading, writing, or have an exceptional condition.  
It returns 0 if the timeout expires before any events occur, or -1 if an error occurs.
1) `fds` is an array of `struct pollfd` structures that contain the file descriptors to monitor and the events to monitor for (see example of the `struct pollfd` bellow in 15.1).
2) `nfds` is the number of file descriptors in the `fds` array.
3) `timeout` specifies the maximum amount of time to wait for an event to occur. If it is -1, the `poll` function will block indefinitely until an event occurs.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <poll.h>
#include <unistd.h>

int main() {
	struct pollfd fds[1];
	int timeout_msecs = 5000;
	int ret;

	// Monitor stdin (fd 0) for input
	fds[0].fd = 0;
	fds[0].events = POLLIN;

	ret = poll(fds, 1, timeout_msecs);
	if (ret == -1) {
		perror("poll");
		return 1;
	}

	if (fds[0].revents & POLLIN) {
		printf("stdin is ready for reading\n");
	} else {
		printf("No data within five seconds.\n");
	}

	return 0;
}
```
The example above monitors the standard input (file descriptor 0) for five seconds to see if any data is available to be read.  
</details>




## struct pollfd
26.1 < < < < < < < < < < < < < < < < < < < STRUCT POLLFD > > > > > > > > > > > > > > > > > > > >

`struct pollfd` is a structure used by the `poll` function to specify the file descriptors to monitor and the events to monitor for.  

```c
struct pollfd {  
	int		fd;			// File descriptor to monitor  
	short	events;		// Events to monitor for (POLLIN, POLLOUT, POLLERR)  
	short	revents;	// Events that occurred (POLLIN, POLLOUT, POLLERR)  
};
```  
- `fd` is the file descriptor to monitor.  
- `events` is a bit mask specifying the events to monitor for. It can be one or more of  the following values:  
	- `POLLIN` - Data available to be read  
	- `POLLOUT` - Data can be written  
	- `POLLERR` - Error condition  
- `revents` is a bit mask specifying the events that occurred.  

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <poll.h>
#include <unistd.h>

int main() {
	struct pollfd fds[2];

	// Monitor stdin (fd 0) for input
	fds[0].fd = 0;
	fds[0].events = POLLIN;

	// Monitor stdout (fd 1) for being ready to write
	fds[1].fd = 1;
	fds[1].events = POLLOUT;

	int ret = poll(fds, 2, 5000);
	if (ret == -1) {
		perror("poll");
		return 1;
	}

	if (fds[0].revents & POLLIN) {
		printf("stdin is ready for reading\n");
	}

	if (fds[1].revents & POLLOUT) {
		printf("stdout is ready for writing\n");
	}

	return 0;
}
```
The example above monitors the standard input (file descriptor 0) for input and the standard output (file descriptor 1) for being ready to write.  
</details>




## select
27 < < < < < < < < < < < < < < < < < < < SELECT > > > > > > > > > > > > > > > > > > > >

`select` function in C is used to monitor multiple file descriptors.  
It allows a program to monitor multiple file descriptors to see if any of them are ready for reading, writing,
or if an exceptional condition (error) has occurred.  

`int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout)`  
Returns the number of file descriptors that are ready for reading, writing, or have an exceptional condition.  
It returns 0 if the timeout expires before any events occur, or -1 if an error occurs.
1) `nfds` specifies the highest-numbered file descriptor in any of the three sets, plus 1.
2) `readfds` is a set of file descriptors to monitor for reading.
3) `writefds` is a set of file descriptors to monitor for writing.
4) `exceptfds` is a set of file descriptors to monitor for exceptional conditions.
5) `timeout` specifies the maximum amount of time to wait for an event to occur. If it is NULL, the `select` function will block indefinitely until an event occurs.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
	fd_set rfds;
	struct timeval tv;
	int retval;

	// Watch stdin (fd 0) to see when it has input.
	FD_ZERO(&rfds);
	FD_SET(0, &rfds);

	// Wait up to five seconds.
	tv.tv_sec = 5;
	tv.tv_usec = 0;

	retval = select(1, &rfds, NULL, NULL, &tv);
	if (retval == -1)
		perror("select");
	else if (retval)
		printf("Data is available now.\n");
	else
		printf("No data within five seconds.\n");

	return 0;
}
```
The example above monitors the standard input (file descriptor 0) for five seconds to
see if any data is available to be read..  
</details>




## fd_set
27.1 < < < < < < < < < < < < < < < < < < < FD_SET > > > > > > > > > > > > > > > > > > > >

`fd_set` is a data type used by the `select` function to specify the file descriptors to monitor.  

```c
typedef struct {
	unsigned long fds_bits[FD_SETSIZE / (8 * sizeof(long))];
} fd_set;
```
`fd_set` is a bit mask that can hold up to `FD_SETSIZE` (1024) file descriptors.




## epoll
28 < < < < < < < < < < < < < < < < < < < EPOLL > > > > > > > > > > > > > > > > > > > >

`epoll` is an interface in Linux for monitoring multiple file descriptors (I/O multiplexing).  
It is similar to the `select` and `poll` functions, but it is more efficient for large numbers of file descriptors.  

INTERFACE FUNCTIONS:  
`int epoll_create(int size)`  
Returns a file descriptor for the new epoll instance, or -1 if an error occurs.

`int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event)`
1) `epfd` is the file descriptor returned by `epoll_create`.
2) `op` specifies the operation to perform. It can be one of the following values:
	- `EPOLL_CTL_ADD` - Add a file descriptor to the epoll instance.
	- `EPOLL_CTL_MOD` - Change the events to monitor for a file descriptor.
	- `EPOLL_CTL_DEL` - Remove a file descriptor from the epoll instance.
3) `fd` is the file descriptor to add, modify, or remove.
4) `event` is a pointer to a `struct epoll_event` structure that specifies the events to monitor for the file descriptor.

`int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout)`
Returns the number of events that occurred, or -1 if an error occurs.
1) `epfd` is the file descriptor returned by `epoll_create`.
2) `events` is an array of `struct epoll_event` structures that will store the events that occurred.
3) `maxevents` is the maximum number of events to store in the `events` array.
4) `timeout` specifies the maximum amount of time to wait for an event to occur. If it is -1, the `epoll_wait` function will block indefinitely until an event occurs.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <sys/epoll.h>
#include <unistd.h>

int main() {
	int epfd = epoll_create1(0);
	if (epfd == -1) {
		perror("epoll_create1");
		return 1;
	}

	struct epoll_event event;
	event.events = EPOLLIN;
	event.data.fd = 0;

	if (epoll_ctl(epfd, EPOLL_CTL_ADD, 0, &event) == -1) {
		perror("epoll_ctl");
		return 1;
	}

	struct epoll_event events[10];
	int num_events = epoll_wait(epfd, events, 10, 5000);
	if (num_events == -1) {
		perror("epoll_wait");
		return 1;
	}

	for (int i = 0; i < num_events; i++) {
		if (events[i].events & EPOLLIN) {
			printf("stdin is ready for reading\n");
		}
	}

	return 0;
}
```
The example above ctreats an epoll instance, adds the standard input (file descriptor 0) to the interest list with `EPOLLIN` event, and then `epoll_wait` is used to wait for `EPOLLIN` event.  
If the standard input is ready for reading, it prints a respective message.  
</details>




## struct epoll_event
28.1 < < < < < < < < < < < < < < < < < < < EPOLL_EVENT > > > > > > > > > > > > > > > > > > > >

`struct epoll_event` is a structure used by the `epoll` interface to specify the events to monitor for and the events that occurred.

```c
struct epoll_event {
	uint32_t events;	// Events to monitor for (EPOLLIN, EPOLLOUT, EPOLLERR)
	epoll_data_t data;	// User data
};
```

- `events` is a bit mask specifying the events to monitor for. It can be one or more of the following values:
	- `EPOLLIN` - Data available to be read
	- `EPOLLOUT` - Data can be written
	- `EPOLLERR` - Error condition

- `data` is a union that can hold either a file descriptor or a pointer to user-defined data.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <sys/epoll.h>
#include <unistd.h>

int main() {
	int epfd = epoll_create1(0);
	if (epfd == -1) {
		perror("epoll_create1");
		return 1;
	}

	struct epoll_event event;
	event.events = EPOLLIN;
	event.data.fd = 0;

	if (epoll_ctl(epfd, EPOLL_CTL_ADD, 0, &event) == -1) {
		perror("epoll_ctl");
		return 1;
	}

	struct epoll_event events[10];
	int num_events = epoll_wait(epfd, events, 10, 5000);
	if (num_events == -1) {
		perror("epoll_wait");
		return 1;
	}

	for (int i = 0; i < num_events; i++) {
		if (events[i].events & EPOLLIN) {
			printf("stdin is ready for reading\n");
		}
	}

	return 0;
}
```

The example above creates an epoll instance, adds the standard input (file descriptor 0) to the interest list with `EPOLLIN` event, and then `epoll_wait` is used to wait for `EPOLLIN` event.

If the standard input is ready for reading, it prints a respective message.
</details>




## kqueue
29 < < < < < < < < < < < < < < < < < < < KQUEUE > > > > > > > > > > > > > > > > > > > >

`kqueue` is an interface in BSD-based operating systems like FreeBSD, OpenBSD, and macOS
used for monitoring multiple file descriptors (I/O multiplexing).  
It is similar to the `epoll` interface in Linux.  

INTERFACE FUNCTIONS:  
`int kqueue(void)`  
it returns a file descriptor for the new kqueue instance, or -1 if an error occurs.

`int kevent(int kq, const struct kevent *changelist, int nchanges, struct kevent *eventlist, int nevents, const struct timespec *timeout)`  
is used to register events to the queue or retrieve pending events from the queue.  
Returns the number of events that occurred, or -1 if an error occurs.  
1) `kq` is the file descriptor returned by `kqueue`.
2) `changelist` is an array of `struct kevent` structures that specifies the changes to make to the kqueue instance (see example of the `struct kevent` bellow in 17.1).
3) `nchanges` is the number of changes to make to the kqueue instance.
4) `eventlist` is an array of `struct kevent` structures that will store the events that occurred.
5) `nevents` is the maximum number of events to store in the `eventlist` array.
6) `timeout` specifies the maximum amount of time to wait for an event to occur. If it is NULL, the `kevent` function will block indefinitely until an event occurs.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/event.h>
#include <sys/time.h>
#include <unistd.h>

int main() {
	int kq = kqueue();
	if (kq == -1) {
		perror("kqueue");
		return 1;
	}

	struct kevent change;
	EV_SET(&change, STDIN_FILENO, EVFILT_READ, EV_ADD, 0, 0, NULL);

	struct kevent event;
	while (1) {
		int ret = kevent(kq, &change, 1, &event, 1, NULL);
		if (ret == -1) {
			perror("kevent");
			return 1;
		}

		if (event.flags & EV_EOF) {
			printf("EOF detected on stdin\n");
			break;
		}

		if (event.filter == EVFILT_READ) {
			char buf[1024];
			ssize_t n = read(STDIN_FILENO, buf, sizeof(buf) - 1);
			if (n > 0) {
				buf[n] = '\0';
				printf("Read: %s", buf);
			}
		}
	}

	return 0;
}
```
The example above creates a kqueue instance, adds the standard input (file descriptor 0) to the interest list with `EVFILT_READ` event, and then `kevent` is used to wait for `EVFILT_READ` event.  
If the standard input is ready for reading, it reads the data from the standard input and prints it to the standard output.  
</details>




## struct kevent
29.1 < < < < < < < < < < < < < < < < < < < STRUCT KEVENT > > > > > > > > > > > > > > > > > > > >

`struct kevent` is a structure used by the `kqueue` interface to specify the events to monitor for and the events that occurred.

```c
struct kevent {
	uintptr_t	ident;	// Identifier for this event
	short		filter;	// Filter for event
	u_short		flags;	// Action flags for kqueue
	u_int		fflags;	// Filter flag value
	intptr_t	data;	// Filter data value
	void		*udata;	// Opaque user data identifier
};
```
- `ident` is an identifier for this event. It can be a file descriptor, process ID, or user-defined identifier.
- `filter` is the filter for this event. It can be one of the following values:
	- `EVFILT_READ` - Read data available
	- `EVFILT_WRITE` - Write data available
	- `EVFILT_AIO` - Asynchronous I/O
	- `EVFILT_VNODE` - File system events
	- `EVFILT_PROC` - Process events
	- `EVFILT_SIGNAL` - Signal events
	- `EVFILT_TIMER` - Timer events
	- `EVFILT_USER` - User-defined events
- `flags` is a bit mask specifying the action to perform. It can be one or more of the following values:
	- `EV_ADD` - Add the event to the kqueue instance.
	- `EV_DELETE` - Remove the event from the kqueue instance.
	- `EV_ENABLE` - Enable the event.
	- `EV_DISABLE` - Disable the event.
	- `EV_ONESHOT` - Only deliver the event once.
	- `EV_CLEAR` - Clear the event state after delivery.
	- `EV_RECEIPT` - Return the event in the `udata` field of the `struct kevent` structure.
- `fflags` is a filter flag value. It depends on the filter specified by the `filter` field.
- `data` is a filter data value. It depends on the filter specified by the `filter` field.
- `udata` is an opaque user data identifier. It is returned in the `udata` field of the `struct kevent` structure.

<details>
<summary>EXAMPLE:</summary>

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/event.h>
#include <sys/time.h>
#include <unistd.h>

int main() {
	int kq = kqueue();
	if (kq == -1) {
		perror("kqueue");
		return 1;
	}

	struct kevent change;
	EV_SET(&change, STDIN_FILENO, EVFILT_READ, EV_ADD, 0, 0, NULL);

	struct kevent event;
	while (1) {
		int ret = kevent(kq, &change, 1, &event, 1, NULL);
		if (ret == -1) {
			perror("kevent");
			return 1;
		}

		if (event.flags & EV_EOF) {
			printf("EOF detected on stdin\n");
			break;
		}

		if (event.filter == EVFILT_READ) {
			char buf[1024];
			ssize_t n = read(STDIN_FILENO, buf, sizeof(buf) - 1);
			if (n > 0) {
				buf[n] = '\0';
				printf("Read: %s", buf);
			}
		}
	}

	return 0;
}
```
The example above creates a kqueue instance, adds the standard input (file descriptor 0) to the interest list with `EVFILT_READ` event using `struct kevent`, and then `kevent` is used in a loop to wait for `EVFILT_READ` event.  
If the standard input is ready for reading, it reads the input and prints it to the standard output.  
If the `EV_EOF` flag is set in the `flags` field of the `struct kevent` structure, it means that the end of file was reached on the file descriptor.  
</details>

