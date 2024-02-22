#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8080

int	main(int argc, char **argv) {

	int 	sock = 0;
	int 	connection;
	long	valread;
	struct sockaddr_in serv_addr;
	const char 	*hello = "Hello from client";
	char 	buffer[1024] = {0};

	// 1. Creating socket, getting its file descriptor
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		
		perror("In socket");
		exit(EXIT_FAILURE);
	}

	// 2. Identifying socket

	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary form
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
		
		perror("In inet_pton");
		exit(EXIT_FAILURE);
	}

	// 3. Connecting to server
	connection = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	if (connection < 0) {
		
		perror("In connect");
		exit(EXIT_FAILURE);
	}

	// 4. Send and receive messages
	send(sock, hello, strlen(hello), 0);
	printf("Hello message sent\n");
	valread = read(sock, buffer, 1024);
	printf("%s\n", buffer);

	return 0;
}

/*
** Compiling: `g++ tcpSocketClientSide.cpp -o clientSide`
** and runnig: `./clientSide`
** !! Should be run after serverSide is running !!
**
** see `tcpSocketServerSide.cpp` for more.
*/
