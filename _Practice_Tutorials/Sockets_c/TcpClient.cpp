/*
** This example shows the client side of a TCP connection.
** The client sends a request to the server and the server responds with a response.
** The client then prints the response to the standard output.
**
** This is how a web browser works.
**
** This IP can be used to test the client:
** 93.184.216.34 // example.com
**
** Usage:
** `/tcpc <IP address>`
*/
#include "Common.hpp"

//#define SERVER_PORT		80 // usual port for HTTP connections for web servers

int	main(int argc, char **argv) {

	int					sock_fd;
	int					n;
	int					send_bytes;
	struct sockaddr_in	servaddr;
	char				sendline[MAXLINE];
	char				recvline[MAXLINE];
	int					return_value;

	if (argc != 2) {
		printErrorAndExit("Usage: %s <IPaddress>", argv[0]);
	}

	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd < 0) {
		printErrorAndExit("Error creating socket!");
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERVER_PORT);

	return_value = inet_pton(AF_INET, argv[1], &servaddr.sin_addr); // convert IP addr from text to binary form
	if (return_value <= 0) {
		printErrorAndExit("inet_pton error for %s", argv[1]);
	}

	//std::cout << "Connecting to " << argv[1] << ":" << SERVER_PORT << std::endl;

	return_value = connect(sock_fd, (SA *)&servaddr, sizeof(servaddr));
	if (return_value < 0) {
		printErrorAndExit("connect error");
	}

	// At this point, the client is connected to the server. Compose the request msg.
	sprintf(sendline, "GET / HTTP/1.1\r\n\r\n");
	send_bytes = strlen(sendline);

	// Send the request to the server / write to the socket
	return_value = write(sock_fd, sendline, send_bytes);
	if (return_value != send_bytes) {
		printErrorAndExit("write error");
	}

	memset(recvline, 0, MAXLINE);

	// Read the response from the server / read from the socket
	while ((n = read(sock_fd, recvline, MAXLINE - 1)) > 0) {

		printf("%s", recvline);
		memset(recvline, 0, MAXLINE);
	}
	if (n < 0) {
		printErrorAndExit("read error");
	}

	exit(EXIT_SUCCESS);
}
